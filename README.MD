# lang_it

Langit means sky in some austronesian languages (malay being one i can speak).

My goal with whatever this is, is to be able to achieve B1 translation between PT and a different language with a compiled binary with less than 500kb, and then move on to different language pairs, more similar or more distant so that i can document the different processes involved, how do they affect the final binary size and how does accuracy compare. 

Eventually i wanna compare pairs as distant as Malay-Japanese and Chinese-Portuguese or as close as Italian-Portuguese and Portuguese-Spanish. 

## Why would i do that to myself

Getting into studies of computational linguistics and machine translation, i wanted to know how far i can get with small amounts of code for a translation program based on rules on lemmas and stems.

My benchmarks will be:

- The book "The little prince". (Full translation) ( ?? %) for vocabulary and context;
- The ESP32 for Portability.

At first, My goal is to introduce the following translations, and then pivot them between each other, so that any can be translated to any other:

| FROM          | TO            | STATUS |  SIZE | The Little Prince | Estimated CEFR Level | Words | Dictionary Size |
| ------------- | ------------- | ------ | ----- | ----------------- | -------------------- | ----- | --------------- |     
| Portuguese    | English       |  ⚠️    | 292kb |       ?? %        |                      |  1565 |       188       |   
| English       | Japanese      |  ⚠️    |       |       ?? %        |                      |       |                 |   
| English       | Portuguese    |  ⚠️    |       |       ?? %        |                      |       |                 |   
| English       | French        |  ⚠️    |       |       ?? %        |                      |       |                 |   
| Spanish       | Portuguese    |  ⚠️    |       |       ?? %        |                      |       |                 |   
| Swedish       | English       |  ⚠️    |       |       ?? %        |                      |       |                 |   

## How to

### Run benchmarks

```sh
cd benchmarks # open benchmark folder
g++ -o ./translate_line ./translate_line.cpp ../pairs/*.cpp # generate binary 
ruby benchmark.rb # run ruby script
cd results # open results folder
ls 
```


### Use the library

Simply import the header file 

```cpp
#include "path/to/lang_it.h"
```

Then call the function corresponding to your desired pair, passing a string:

```cpp
string texto = "teste";
string traducao = traduzir_en(test);
cout << traducao;
```