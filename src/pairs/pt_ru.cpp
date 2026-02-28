#include "../lang_it.h"
#if defined(ALL)
#define FROM "pt"
#define TO "ru"
#define PT_RU

#endif

#ifdef PT_RU

#include <string>
#include <vector>
#include <iostream>

using namespace std;


Info pt_ru_info = {
    SVO, {0},
    SVO, {1}
};





CASE_DEF(ptru_case_to, SUFFIX,
{
  {ACCUSATIVE, FEMININE_GENDER, "а", "у"},   // собака → собаку
    {ACCUSATIVE, FEMININE_GENDER, "я", "ю"},   // неделя → неделю
    {ACCUSATIVE, ON, "", ""},    // мужчина → мужчину handled separately maybe
    {ACCUSATIVE, NEUTRAL_GENDER, "о", "о"},    // окно → окно (same for accusative)
   {ACCUSATIVE, NEUTRAL_GENDER, "е", "е"}     // море → море
});

DICT(fixed_ngrams, {
 {"por_exemplo", "например"},
 
});

DICT(adj, {
 {"quiet", "тих"},
 {"vazi", "пуст"}
});

DICT(nouns, {
 {"olá", "привет"},
 {"cobra", "змея"},
 {"cachorr", "собака", FEMININE_GENDER},
 {"familia", "семья", FEMININE_GENDER},
 {"gat", "кот"},
 {"hoje", "сегодня"},
 {"amanhã", "завтр",FEMININE_GENDER},
 {"livro", "книга",FEMININE_GENDER},
 {"mundo", "мир"},
 {"noite", "ночь"},
 {"açucar", "сахар"},
 {"leite", "молоко"},
 {"criança", "ребенок"},
 {"grama", "трава", FEMININE_GENDER, FEMININE_GENDER},
 {"tradutor", "переводчик",NEUTRAL_GENDER},
 {"manga", "манго"},
 {"juice", "сок"},
 {"camisa", "рубашка"}
});

DICT(pro, {
 {"eu", "я"},
 {"você", "ты"},
 {"sou", ""}
});

DICT(obl_pro, {
 {"te", "тебя"},
 {"me" "себя"}
});



DICT(adv, {
 {"também", "тоже"},
 {"e", "и"}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"as", "as"},
 {"os", "os"}
});

V_DICT(verbs, {
    {"sab", "зна"},
    {"v", "виде"},
    {"funcio", "работа"}
});

VERB_ENDINGS(pt_ru_reg, {
   {{"ou", "ei", "aram", "eu", "i"}, PAST_TENSE},
   {{"ar", "er", "ir"}, INFINITIVE},
   {{"jo", "no", "na", "ne"}, PRESENT_TENSE},
   {{""}, IRREGULAR}
});

VERB_CONJUGATIONS(pt_ru_def, 
{
   {INFINITIVE, SUFFIX, "ть"},
   {PAST_TENSE, SUFFIX, "л"},
   {PRESENT_TENSE, SUFFIX, "ет"}
}
);

SUFFIX_RULES(suff, {
  {"mente", "о", ADVERB},
   {"ção", "ция", NOUN},
   {"ções", "ции", NOUN},
   {"dade", "ность", NOUN},
   {"ismo", "изм", NOUN},
   {"ista", "ист", NOUN},
   {"al", "альный", ADJECTIVE},
   {"ico", "ический", ADJECTIVE},
   {"logia", "логия", NOUN}
});


HOMONYM_DEF(
    manga,
    HOMONYM_OUTCOMES(
        { "манго", 0 },
        { "рукав", 0 }
    ),
    HOMONYM_FORBIDDEN(98, 99),
    "сок","$", "рубашка"
);

Homonym pt_ru_homonyms[] = {
    HOMONYM("manga", manga)
};


  GENDER_DEF(ptru_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });

    GENDER_DEF(ptru_gender_to, SUFFIX,
    {
       {NONE, "ий"},
       {FEMININE_GENDER, "ая"},
       {NEUTRAL_GENDER, "ое"}
    });

     PLURAL_DEF(ptru_plural_from, SUFFIX,
        {
           {NONE, "", "s"}
        });

    PLURAL_DEF(ptru_plural_to, SUFFIX,
    {
       {NEUTRAL_GENDER, "о", "а"},
       {FEMININE_GENDER, "я", "и"},
       {FEMININE_GENDER, "ь","и"},
       {FEMININE_GENDER, "а", "ы"}
    });





static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("tec", REPLACE_START, "тех");
    NORMALIZE("b", REPLACE_ALL, "б");
    NORMALIZE("c", REPLACE_ALL, "к");
    NORMALIZE("d", REPLACE_ALL, "д");
    NORMALIZE("f", REPLACE_ALL, "ф");
    NORMALIZE("g", REPLACE_ALL, "г");
    NORMALIZE("h", REPLACE_ALL, "х");
    NORMALIZE("j", REPLACE_ALL, "ж");
    NORMALIZE("k", REPLACE_ALL, "к");
    NORMALIZE("l", REPLACE_ALL, "л");
    NORMALIZE("m", REPLACE_ALL, "м");
    NORMALIZE("n", REPLACE_ALL, "н");
    NORMALIZE("p", REPLACE_ALL, "п");
    NORMALIZE("r", REPLACE_ALL, "р");
    NORMALIZE("s", REPLACE_ALL, "с");
    NORMALIZE("t", REPLACE_ALL, "т");
    NORMALIZE("v", REPLACE_ALL, "в");
    NORMALIZE("w", REPLACE_ALL, "в");
    NORMALIZE("x", REPLACE_ALL, "кс");
    NORMALIZE("y", REPLACE_ALL, "и");
    NORMALIZE("z", REPLACE_ALL, "з");
    NORMALIZE("a", REPLACE_ALL, "а");
    NORMALIZE("e", REPLACE_ALL, "е");
    NORMALIZE("i", REPLACE_ALL, "и");
    NORMALIZE("o", REPLACE_ALL, "о");
    NORMALIZE("u", REPLACE_ALL, "у");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()
     RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")
     
     RULE("IF OBLIQUE_PRONOUN THEN VERB DO INVERT")

     

     DEFAULT()

    }

    CLEANUP(reordered_arr);
    
    HANDLE_CASE(NO_CASE,&ptru_case_to)
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}


final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, pt_ru_homonyms);

return final_arr;
}


static Word nounLookup(const string& word) {
 
  

    LOOKUP(nouns, NOUN, word, &ptru_gender_from, NO_GENDER, &ptru_plural_from, &ptru_plural_to);
    
    LOOKUP(adj, ADJECTIVE, word, &ptru_gender_from,&ptru_gender_to, &ptru_plural_from, &ptru_plural_from);

    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(obl_pro, OBLIQUE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    
    SUFFIX_LOOKUP(suff, word, adj);

    VERB_LOOKUP(verbs, word, pt_ru_reg, pt_ru_def);


    return { word, word, -1 };
}

MAIN(pt_ru, fixed_ngrams, reorder_helpers, nounLookup)

#endif