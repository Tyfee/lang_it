# lang_it

Langit means sky in some austronesian languages.

lang_it is a rule-based, portable and lightweight universal translation engine written using only C++, with support for:

- Multi word entries
- Verb conjugation
- Inflexion (Gender, number, case, etc)
- Homonym disambiguation
- Auto-correct
- Multi-script languages (Latin, Hanzi, Kanji and Kana, Arabic-based, Cyrilic, Hangul)
- Validated JS (WASM), Ruby (Rice) and Java (JNI) bindings. 

## Why?

In the age of ML based translation, why care about deterministic rule-based translation programs? 
For me personally i see the values in two aspects:

- Endangered languages with little validated corpora available for ML model training, but reasonable research on the grammar and vocabulary can be found.
- Linguistics analysis: lang_it can also produce tagged translations that can be used to generate statistics and reports on the language is translation into by seeing how does sentence in language A turns into sentence in language B.
- Portability: lang_it is super small compared to modern translation tools, and with enough module tuning, you can get very accurate translations, even with MCUs. You ever wanted your fridge to translate offline? now it can!

## How?

Here's a brief explanation on how lang_it works:

The whole engine exists within the header file, and every module written to work with it, needs to have it included and use it's functions and macros.

It has multiple layers that do operations to locate words in a dictionary, apply inflection before and after, apply deterministic rules, convert script, normalize words and etc.

Roughly like this:

`Input > Tokenize > Lookup n-grams [3, 2] > Individual word Lookup > Dictionary Lookup with inflexion > Optional auto-correct > Apply inflection/morphology to target language > Apply deterministic rules > Normalize common patterns > Homonym disambiguation > Output`

Now, there are two different ways of creating a translation module compatible with lang_it:

- Raw C++ code with macros and compiled against the header file.
- Compiled binary that can be loaded into the program.

With the first method, you will get a faster translation, but a bigger binary size as well as more freedom with rules, since you can implement them with pure code. But it takes more effort over all to get it working.

For example, to sketch a malay to chinese translator:

First create a file called ma_zh.cpp inside the pairs folder. 

Then inside we add
```cpp
#include "../lang_it.h"　// Include the core library
#if defined(ALL)
#define MA_ZH // arbitrarily choose a name for the pair and define it, FROM_TO is enough
#endif

#ifdef MA_ZH // wrap the whole code in a definition check

#include <string>
#include <vector>

using namespace std;


DICT(fixed_ngrams, {
 {"", ""},
});

//create a basic dictionary mapping {word, translation}
DICT(nouns, {
 {"kucing", "猫"},
 {"anjing", "狗"},
 {"air", "水"},
 {"cendawan", "蘑菇"}
 }); 

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     DEFAULT()
    }

    CLEANUP(reordered_arr);

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
return final_arr;
}


static Word nounLookup(const string& word) {
    // add a simple lookup macro:
    //looking for an entry in the 'nouns' dict, if found assign NOUN type, and the word to look for (which is a token in a sentence we typed), since neither malay nor chinese uses grammatical gender, we pass null pointers.

    LOOKUP(nouns, NOUN, word, (Gender*)nullptr, (Gender*)nullptr);
    return { word, word, -1 };
}

MAIN(ma_zh, fixed_ngrams, reorder_helpers, nounLookup)
#endif
```

Then, in the actual library code, you make it able to be found by its codes:

```cpp
// ADD THIS AT THE TOP

#if defined(MA_ZH) || defined(ALL)
std::string ma_zh(const char* sentence, int script); // the function here, is the first argument we put in our MAIN() macro in the module code

//ADD THIS INSIDE THE translate() function

  #if defined(MA_ZH) || defined(ALL)
      if ((f == "ma") && (t == "zh")) {
          return ma_zh(sentence, script); 
      }


#endif
```
Now if i compile a program with the header and the cpp module as well:

```sh
g++ -DMA_ZH -I ./src/ ./test.cpp ./src/pairs/*.cpp -o test
```

Now if we a write a program that calls:

```cpp
string translation = translate("cendawan, air", "ma", "zh");
std::cout << translation;
```

We get printed:

```
蘑菇, 水
```
----

With the second method, you get a lighter compiled program (just the core code), plus a small binary (.lang) file that can be loaded only when you need, which uses slightly more RAM, but it's also way easier to set up and develop on top.


The compiler is still super limited, but its my current focus of development right now. There's also a GUI to create modules faster in development but in the moment, you need to create your own json files and compile them manually.

For example, to create the same module we created above, we need a json file like:

ma_zh.json: 
```json
{
  "dictionary": [
    {
      "entry": "kucing",
      "translation": "猫",
      "word_type": 0
    },
    {
      "entry": "anjing",
      "translation": "狗",
      "word_type": 0
    },
    {
      "entry": "air",
      "translation": "水",
      "word_type": 0
    },
     {
      "entry": "cendawan",
      "translation": "蘑菇",
      "word_type": 0
    }
  ]
}
```

You then run the compiler, with the name that you want for your module and the path of your json file:

```sh
./compiler test.lang test.json
```

That will generate the file `test.lang`


This file can now be loaded (however it is that youre doing it) into the engine using the function `load_from_bin(const uint8_t* file, size_t size)`. You load it however you want, but you need to feed into the function the whole buffered file, and it's size:

C++ example:

```cpp 
    std::ifstream file(argv[2], std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cout << "Failed to read file\n";
        return 1;
    }

    
   load_from_bin(buffer.data(), buffer.size()); //load the buffer and the size

   cout << translate_from_bin("air, chendawan") // then you call the translation
```

WASM Javascript example:

```js
const response = await fetch(fileUrl);
const arrayBuffer = await response.arrayBuffer();
fileBuffer = new Uint8Array(arrayBuffer);
Module.translate_from_bin("air, chendawan", fileBuffer); // in the wasm bindings, you load and translate with the same function, i know its a lof of useless overhead but i'm not well versed enough in wasm stuff to find out a better way, if i keep them separate i lose the context.
```

## Current State

✅ = WORKS ACROSS MULTIPLE LANGUAGES

🆗 = FUNCTIONAL, BUT NOT FULLY

⚠️ = IMPLEMENTED, BUT ONLY FOR SPECIFIC CASES

❌ = NOT IMPLEMENTED

| FEATURE          |  STATUS    |  TESTED WITH | 
| -------------    |  ------    | -----        | 
| Word Lookup      |   ✅       | English, Portuguese, Japanese, Spanish, French, Chinese, Malay, others.|  
| Auto Correct     |   ⚠️       | Portuguese, English|    
| Gender Inflection|   ✅       | Portuguese, Russian, Spanish|    
| Multi Script     |   🆗       | Japanese (Kana/Kanji/Romaji), Chinese (Hanzi/pīnyīn), Malay(Rumi, Jawi)|  
| Grammatical Case |   ⚠️       |   Portuguese, Russian  |   

-----

lang_it Personal Use License v1.0

Permission is granted to use, modify, and distribute this software
for personal, educational, and research purposes.

Commercial use, including but not limited to embedding in products,
offering paid services, or redistribution in commercial contexts,
requires a separate commercial license agreement.