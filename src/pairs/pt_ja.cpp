#include "../lang_it.h"
#if defined(ALL)
#define FROM "pt"
#define TO "ja"
#define PT_JA

#endif

#ifdef PT_JA

#include <string>
#include <vector>

using namespace std;


Info pt_ja_info = {
    SVO,
    SOV, 
    {0},
    {1}
};





CASE_DEF(ptja_case_to, NEXT_WORD,
{
    {ACCUSATIVE, ON, "", "を"},   
 
});

DICT(fixed_ngrams, {
 {"por_exemplo", "例えば"},
 
});

DICT(adj, {
 {"quiet", "静か"},
 {"vazi", "空っぽ"},
 {"grande", "大"}
});

DICT(nouns, {
 {"olá", "こんにちは"},
 {"cobra", "蛇"},
 {"cachorr", "犬", 0, ANIMATE},
 {"gat", "猫", 0, ANIMATE},
 {"laranja", "オレンジ"},
 {"animal", "動物", 0, ANIMATE},
 {"pessoa", "人", 0, ANIMATE},
 {"familia", "家族"},
 {"ontem", "昨日"},
 {"hoje", "今日"},
 {"aniversário", "誕生日"},
 {"é", "は"},
 {"sou", "は"},
 {"estou", "は"},
 {"está", "は"},
 {"são", "は"}
});

DICT(pro, {
 {"eu", "私"},
 {"ela", "彼女"},
 {"você", "あなた"}
});

DICT(obl_pro, {
 {"te", "あなたを"},
 {"me" "私を"}
});

DICT(poss_pro, {
 {"meu", "私の"},
 {"seu" "あなたの"}
});


DICT(adv, {
 {"também", "も"},
 {"e", "と"},
 {"se", "なら"}
});

DICT(art, {
 {"o", "o"},
 {"a", "a"},
 {"as", "as"},
 {"os", "os"}
});

V_DICT(verbs, {
    {"sab", "知"},
    {"v", "見"},
    {"funcio", "効"},
    {"corr", "走"}
});

VERB_ENDINGS(pt_ja_reg, {
   {{"ou", "ei", "aram", "eu", "i", "imos", "iu"}, PAST},
   {{"ar", "er", "ir"}, INFINITIVE},
   {{"jo", "no", "na", "ne"}, PRESENT},
   {{"endo", "ando", "indo"}, CONTINUOUS}
});

VERB_CONJUGATIONS(pt_ja_def, 
{
   {INFINITIVE, SUFFIX, "", "る"},
   {PAST, SUFFIX, "","た"},
   {PRESENT, SUFFIX, "","っている"},
   {CONTINUOUS, SUFFIX, "","っている"}
}
);

SUFFIX_RULES(suff, {
  {"mente", "的", ADVERB},
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

Homonym pt_ja_homonyms[] = {
    HOMONYM("manga", manga)
};


  GENDER_DEF(ptja_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });


PLURAL_DEF(ptja_plural_from, SUFFIX,
    {
        {NONE, "is", "l"},  // ends with "is" → remove "is", add "l" (animais → animal)
        {NONE, "es", "s"},  // ends with "es" → remove "es", add "s" (leões → leão)
        {NONE, "s", ""}      // ends with "s" → remove "s", add nothing (gatos → gato)
    });
    PLURAL_DEF(ptja_plural_to, SUFFIX,
    {
       {ANIMATE, "", "たち"},
    });





static string normalize(string word) {
    string normalized_ = word;
    NORMALIZE("ode", REPLACE_ALL,"オード");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()

          RULE("IF ARTICLE THEN NOUN DO REMOVE_FIRST")
          RULE("IF NOUN THEN ADJECTIVE DO INVERT")
          
     DEFAULT()

    }
 
    CLEANUP(reordered_arr);
    

    HANDLE_CASE(&pt_ja_info, NO_CASE, &ptja_case_to)

    
    
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}


final_arr = INSERT_PARTICLES(final_arr, {
    {PRONOUN, NOUN, "は", (final_arr.size() > 2)},
    {NOUN, NOUN, "は", (final_arr.size() > 2)},
    {PRONOUN, VERB, "は", (final_arr.size() == 2)}
});
final_arr = MEDIATE_HOMONYMS(final_arr, {"manga", "banco"}, pt_ja_homonyms);


return final_arr;
}


static Word nounLookup(const string& word) {
 
  

    LOOKUP(nouns, NOUN, word, &ptja_gender_from, NO_GENDER, &ptja_plural_from, &ptja_plural_to);
    
    LOOKUP(adj, ADJECTIVE, word, &ptja_gender_from,NO_GENDER, &ptja_plural_from, &ptja_plural_from);

    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    LOOKUP(poss_pro, POSSESSIVE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(obl_pro, OBLIQUE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(adv, ADVERB, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    
    SUFFIX_LOOKUP(suff, word, adj);

    VERB_LOOKUP(verbs, word, pt_ja_reg, pt_ja_def,false);


    return { word, word, -1 };
}

MAIN(pt_ja, fixed_ngrams, reorder_helpers, nounLookup, false,true, pt_ja_reg)

#endif