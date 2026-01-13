#include "../lang_it.h"

#if defined(ALL)
#define ZH_EN
#endif

#ifdef ZH_EN

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;


DICT(fixed_ngrams, {
    {"我_们", "we"},
    {"我_的", "my"},
    {"你_好", "hello"},
    {"世_界", "world"}
});


// adjectives
DICT(adj, {
  {"小", "little"}
  
});

DICT(nouns, {
  {"狗", "dog"},
  {"猫", "cat"},
  {"爱", "love"}
});

//specific entries for particles cause for silly ol me, the line between prepositions and other types of particles is blurr-ay;
DICT(part, {
  {"和", "and"},
  {"不", "no"},
  {"是", "is"}
});

DICT(pro, {
  {"我", "i"},
  {"你", "you"}
});

V_DICT(verbs,{
  {"吃", "eat", 0}
});


//normalization
//this will turn sets of letters that shift on translation and change them accordingly.
// stuff such as aceitar -> aceipt -> accept
static string normalize(string word) {
    string normalized_ = word;

    if (word.length() > 3) {
     
    }



    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy) {
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    int word_count = sentence_arr.size();


    for (size_t i = 0; i < sentence_arr.size(); ++i) {
        bool one_ = (i > 0);
        bool two_ = (i >= 1);
        bool three_ = (i >= 2);

        const Word& current = sentence_arr.at(i);
        const Word* previous = two_ ? &sentence_arr.at(i - 1) : nullptr;
        const Word* previous_ = three_ ? &sentence_arr.at(i - 2) : nullptr;



    }
    return copy;
}

static Word nounLookup(const std::string& word) {
    string translation;
    int word_type = -1;
    
    LOOKUP_BLOCK(nouns, NOUN, word.c_str());
    LOOKUP_BLOCK(adj, ADJECTIVE, word.c_str());
    LOOKUP_BLOCK(pro, PRONOUN, word.c_str());
    LOOKUP_BLOCK(part, PARTICLE, word.c_str());
    
    // If we get here, nothing was found
    return { word, word, -1 };
}


std::string translate_zh(const char* sentence) {
    char buffer[250];
    strncpy(buffer, sentence, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    vector<string> arr = tokenize_cjk(string(buffer));
    std::string translated = trigramLookup(fixed_ngrams, arr, reorder_helpers, nounLookup, false); //true for continuous non-spaced char languages
    return translated;
}

#endif