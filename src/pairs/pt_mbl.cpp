/** esse é um experimento usando o par maxakali -> português */
#include "../lang_it.h"
#if defined(ALL)
#define PT_MBL
#endif

#ifdef PT_MBL

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;


DICT(fixed_ngrams, {
 {"como_sempre", "ehe"},
 
});


//pra definir um dicionario, precisamos da construção DICT(nome, {{"palavra", "tradução"}, {"palavra", "tradução"}})
// nosso dicionario de adjetivos recebe um nome arbitrario "adj" e qualquer lugar onde a palavra "vazio"
//for encontrada, ela será traduzida como "hãmok"
DICT(adj, {
 {"vazio", "hãmhok"}
});

DICT(nouns, {
 {"cobra", "kãyã"},
 {"hoje", "hõnhã"},
 {"amanhã", "hãptup putut"},
 {"mundo", "hãhãm"},
 {"noite", "ãmũy"},
 {"açucar", "axok"}
});

DICT(pro, {
 {"eu", "ãte"}
});


DICT(adv, {
 {"também", "kama"}
});


V_DICT(verbs, {
    {"empurr", "kaxix"},
    {"ajud", "ãyonat"},
    {"esfri", "ãxi"}
});

VERB_CONJUGATION(reg, {
   {{"ou", "ei", "aram"}, PAST_TENSE},
   {{"ar", "er", "ir"}, INFINITIVE},
   {{"o", "a", "e"}, PRESENT_TENSE},
   {{""}, IRREGULAR}
});


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

static Word nounLookup(const string& word) {

    LOOKUP(nouns, NOUN, word);
    
    LOOKUP(adj, ADJECTIVE, word);

    LOOKUP(pro, PRONOUN, word);

    LOOKUP(adv, ADJECTIVE, word);

    VERB_LOOKUP(verbs, word, reg);

    return { word, word, -1 };
}



std::string pt_mbl(const char* sentence) {
    char buffer[250];
    strncpy(buffer, sentence, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    to_lower(buffer);
    vector<string> arr = tokenize(string(buffer));
    std::string translated = trigramLookup(fixed_ngrams, arr, reorder_helpers, nounLookup);

    return translated;
}

#endif