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


constexpr Entry fixed_ngrams[] = {
  {"de_novo", "again"},
  {"我_们",  "we"},
  {"我_的",  "my"},
  {"你_好",  "hello"},
  {"世_界",  "world"}
};



// adjectives
constexpr Entry adj[] = {
  {"小", "little"}
};

constexpr Entry nouns[] = {
  {"狗", "dog"},
  {"猫", "cat"},
  {"爱", "love"}
};

//specific entries for particles cause for silly ol me, the line between prepositions and other types of particles is blurr-ay;
constexpr Entry part[] = {
  {"和", "and"},
  {"不", "no"},
  {"是", "is"}
};


constexpr Entry pro[] = {
  {"我", "i"},
  {"你", "you"}
};

constexpr Verb verbs[] = {
  {"吃", "eat", 0}
};


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
    // TODO: Creaate hierarchy for word category
    string translation;
    // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
    int word_type = -1;



    // for each individual word loop, you look in the noun dictionary
    //first with accentuation, 
    if (lookup(nouns, word.c_str())) {
        translation = lookup(nouns, word.c_str());
        word_type = 0;
    }
    else if (lookup(adj, word.c_str())) {

        translation = lookup(adj, word.c_str());
        word_type = 1;

    }
    else if (lookup(pro, word.c_str())) {
        translation = lookup(pro, word.c_str());
        word_type = 4;
        
    }
     else if (lookup(part, word.c_str())) {
        translation = lookup(part, word.c_str());
        word_type = 67; //random
        
    }
    else {
        return { word, word, -1 };

    }
    return { word, normalize(translation), word_type };
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