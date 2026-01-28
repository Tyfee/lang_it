/** esse é um experimento usando o par maxakali -> português */
/** this is an experiment using the maxakali -> portuguese pair */

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

DICT(adj, {
 {"vazio", "hãmhok"},                          // SYNONYMS(traduz_para, um, dois, tres)  
 SYNONYMS("yĩxux", "azul", "amarelo", "verde") // nos dicionarios maxakali as palavras azul, amarelo, verde
                                                // sao todas traduzidas como yĩxux
});

DICT(nouns, {
 {"cobra", "kãyã"},
 {"hoje", "hõnhã"},
 {"amanhã", "hãptup putut"},
 {"mundo", "hãhãm"},
 {"noite", "ãmũy"},
 {"açucar", "axok"},
 {"leite", "xokhep"},
 {"criança", "kakxop"}
});

DICT(pro, {
 {"eu", "ãte"},
 SYNONYMS("ha", "ele", "ela", "isso")
});


DICT(adv, {
 {"também", "kama"}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"os", "os"},
 {"as", "as"}
});

V_DICT(verbs, {
    {"empurr", "kaxix"},
    {"ajud", "ãyonat"},
    {"esfri", "ãxi"},
    {"beb", "xo'op"},
    {"d", "hõm"}
});

VERB_ENDINGS(reg, {
   {{"ou", "ei", "aram", "eu"}, PAST_TENSE},
   {{"ar", "er", "ir"}, INFINITIVE},
   {{"o", "a", "e"}, PRESENT_TENSE},
   {{""}, IRREGULAR}
});

VERB_CONJUGATIONS(def, 
{
   {INFINITIVE, NONE, ""},
}
);


static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("rr", REPLACE_ALL, "h");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     // maxacali não possui artigos, logo se um artigo é seguido por um substantivo, removemos ele
     // maxacali does not use articles, so if we have a noun following an article, we remove it
     RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")

     // a ordem do português é SVO, já a do maxakalí na maioria das vezes é SOV
     // logo se temos um verbo e depois um substantivo em portugues, invertemos a ordem S[VO] -> S[OV]
     RULE("IF VERB THEN NOUN DO INVERT")

     DEFAULT()
    }

    CLEANUP(reordered_arr)

vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}

    return final_arr;
}


static Word nounLookup(const string& word) {

    LOOKUP(nouns, NOUN, word);
    
    LOOKUP(adj, ADJECTIVE, word);

    LOOKUP(pro, PRONOUN, word);

    LOOKUP(adv, ADJECTIVE, word);

    LOOKUP(art, ARTICLE, word);

    VERB_LOOKUP(verbs, word, reg, def);

    return { word, word, -1 };
}

MAIN(pt_mbl, fixed_ngrams, reorder_helpers, nounLookup)

#endif