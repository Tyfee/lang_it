#include "../lang_it.h"
#if defined(ALL)
#define FROM "pt"
#define TO "es"
#define PT_ES

#endif

#ifdef PT_ES

#include <string>
#include <vector>
#include <iostream>

using namespace std;


Info PT_ES_info = {
    SVO,
    {0},
    SVO, 
    {1}
};





CASE_DEF(ptes_case_to, SUFFIX,
{
  {ACCUSATIVE, FEMININE_GENDER, "а", "у"},   // собака → собаку
    {ACCUSATIVE, FEMININE_GENDER, "я", "ю"},   // неделя → неделю
    {ACCUSATIVE, ON, "", ""},    // мужчина → мужчину handled separately maybe
    {ACCUSATIVE, NEUTRAL_GENDER, "о", "о"},    // окно → окно (same for accusative)
   {ACCUSATIVE, NEUTRAL_GENDER, "е", "е"}     // море → море
});

DICT(fixed_ngrams, {
 {"", ""},
 
});

DICT(nouns, {
  {"olá", "hola"},
  {"oi", "hola"},
  {"dois", "dos"},
  {"duas", "dos"},
  {"seis", "six"},
  {"seven", "seven"},
  {"oito", "eight"},
  {"nove", "nine"},
  {"dez", "ten"},
  {"cem", "a hundred"},
  {"mil", "a thousand"},
  {"segundo", "second"},
  {"ano", "año"},
  {"mês", "month"},
  {"hora", "hour"},
  {"leite", "leche"},
  {"meia", "calcetín"},
  {"rua", "calle"},
  {"onda", "ola"},
  {"praia", "playa"},

  {"coração", "corazón"},
  {"árvore", "árbol"},
  {"loja", "tienda"},
  {"janela", "ventana"},
  {"varanda", "balcón"},
  {"casal", "pareja"},
  {"meia", "calcetín"},
  {"rua", "calle"},

  {"banheiro", "baño"},
  {"cozinha", "cocina"},
  
  {"melancia", "sandía"},
  {"acucar", "sugar"},
  {"sal", "salt"},
  {"cadeira", "silla"},
  {"mãe", "madre"},
  {"pai", "padre"}
});

DICT(art, {
  {"o", "el"},
  {"os", "los"},
  {"a", "la"},
  {"um", "un"},
  {"uma", "una"}
});

DICT(pre, { 
  {"do", "del"},
  {"da", "de la"},
  {"de", "de"},
  {"ou", "o"},
  {"em", "en"},
  {"no", "en el"},
  {"na", "en la"},
  {"nos", "en los"},
  {"nas", "en las"},
  {"à", "to"},
  {"às", "to the"},
  {"ao", "to the"},
   // these verbs are grounded here for misbehaving until second order  
  {"é", "és"},
  {"sou", "soy"},
  {"num", "en un"}
});

// nominative/personal pronouns
DICT(pro, {
  {"eu", "yo"},
  {"você", "tu"},
  {"nós",  "we"},
  {"ele",  "él"},
  {"ela",  "ella"},
  {"elas",  "ellas"},
  {"eles", "ellos"}
  
});
DICT(poss_pro, {
  {"seu", "tu"},
  {"dela", "su"},
  {"dele",  "su"},
  {"nosso",  "nuestro"},
  {"meu",  "mi"},
  {"meus", "mis"},
  {"minha",  "mi"},
  {"minhas",  "mis"}
});

//object pronoun match (in english)

DICT(obj_pro, {
  {"ella", "la"},
  {"él", "lo"},
  {"ellos", "los"},
  {"tu", "te"},
});



// adjectives
DICT(adj,  {
  {"vermelho", "rojo"},
  {"bonito", "belo"},
  {"legal", "genial"},
  {"maior", "más grande"},
  {"engraçado", "gracioso"},
  {"sozinho", "solo"},
  {"dificil", "duro"},
  {"bem", "bien"},
  {"bom", "bueno"},
  {"ruim", "malo"},
  {"mal", "malo"},
  {"mau", "malo"},
  {"humido", "humid"},
  {"pior", "peor"},
  {"estranho", "raro"},
  {"esquisito", "raro"},
  {"doente", "enfermo"},
  {"outro", "otro"},
  {"gratis", "free"},
  {"doce", "dulce"},
  {"azedo", "ácido"},
  {"capaz", "capable"},
  {"louco", "crazy"},
  {"perto", "close"},
  {"pesado", "heavy"},
  {"torto", "bent"}
});

//adverbs

DICT(adv, {
  {"se", "si"},
  {"talvez", "tal vez"},
  {"tomara", "ojalá"},
  {"mas", "pero"},
  {"enquanto", "mientras"},
  {"onde", "donde"},
  {"e", "y"},
  {"ali", "there"},
  {"ninguém", "nadie"},
  {"até", "hasta"},
  {"muito", "very"},
  {"já", "ya"},
  {"mais", "más"},
  {"fora", "outside"},
  {"hoje", "hoy"},
  {"ontem", "ayer"},
  {"amanhã", "mañana"},
  {"agora", "ahora"},
  {"depois", "despues"},
  {"pra", "para"},
  {"por", "por"},
  {"sobre", "acerca de"},
  {"ainda", "aún"},
  {"somente", "solamente"},
  {"só", "solo"},
  {"não", "no"},
  {"sim", "sí"},
  {"apenas", "solamente"},
  {"então", "entonces"}
});


V_DICT(verbs, {
  {"fal", "habl", false, false},
  {"gost", "gust", false, DATIVE_CONST},
  {"com", "com", false, false},
  {"quer", "quier"}
});

#define AR_VERB FREE_BIT_1  // 0x0060
#define ER_VERB FREE_BIT_2  // 0x0070
#define IR_VERB FREE_BIT_1  // Could reuse if mutually exclusive

// DEBUG: Are we getting here?
VERB_ENDINGS(PT_ES_reg, {
   // Infinitive endings - just tense (no person/number)
   {{"ar"}, INFINITIVE | AR_VERB},  // 0x2000
   {{"er"}, INFINITIVE | ER_VERB},  // 0x2000
   {{"ir"}, INFINITIVE | IR_VERB},  // 0x2000
   {{"ei"}, PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR},  // 0x6010?
   {{"i"},  PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR },  // for -ir verbs
   {{"ou"}, PAST | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR},
   {{"aram"}, PAST | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | PLURAL_V},
   {{"o"}, PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR},
   {{"as"}, PRESENT | INDICATIVE | SIMPLE | ACTIVE | SECOND_PERSON | SINGULAR},
   {{"a"}, PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR},
});

VERB_CONJUGATIONS(PT_ES_def, 
{
   // Infinitive mapping (Portuguese infinitive → Spanish infinitive)
   {INFINITIVE | AR_VERB, SUFFIX, "", "ar"},  // hablar
   {INFINITIVE | ER_VERB, SUFFIX, "", "er"},  // comer
   {INFINITIVE | IR_VERB, SUFFIX, "", "ir"},  // vivir
   
   // Past tense 1st person singular
   {PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR, 
    SUFFIX, "", "é"},  // hablé
   {PAST | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR, 
    SUFFIX, "i", "í"},   // for -ir verbs: partí
   
   // Past tense 3rd person singular
   {PAST | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR, 
    SUFFIX, "ou", "ó"},  // habló
   
   // Present tense 1st person singular
   {PRESENT | INDICATIVE | SIMPLE | ACTIVE | FIRST_PERSON | SINGULAR, 
    SUFFIX, "o", "o"},   // hablo
   
   // Present tense 3rd person singular
   {PRESENT | INDICATIVE | SIMPLE | ACTIVE | THIRD_PERSON | SINGULAR, 
    SUFFIX, "", "a"},   // habla
});

SUFFIX_RULES(suff, {
  {"inho", "ito",0, 0},
  {"inha", "ita", 0, 0},
  {"mento", "ment", 0, 0},
  {"ável", "able", 1, 0},
  {"ível", "ible", 1, 0},
  {"ória", "ory", 0, 0},
  {"cidade", "cidad", 0, 0},
  {"tor", "ctor", 0,0},
  {"ais", "ales", 0, 1},
  {"eito", "ecto", 1,0},
  {"ês", "és", 0, 0},
  {"ção", "ción", 0,0},
  {"ções", "ciónes", 0,0},
});



  GENDER_DEF(ptes_gender_from, SUFFIX,
        {
           {NONE, "o"},
           {FEMININE_GENDER, "a"}
        });

    GENDER_DEF(ptes_gender_to, SUFFIX,
    {
       {NONE, "o"},
        {FEMININE_GENDER, "a"}
    });

     PLURAL_DEF(ptes_plural_from, SUFFIX,
        {
           {NONE, "s", ""}
        });

    PLURAL_DEF(ptes_plural_to, SUFFIX,
    {
        {NONE, "s", ""}
    });





static string normalize(string word) {
    string normalized_ = word;


         NORMALIZE("ch", REPLACE_ALL, "ll");
            NORMALIZE("qua", REPLACE_START, "cua");
            NORMALIZE("lh", REPLACE_ALL, "j");
            NORMALIZE("ça", REPLACE_ALL, "za");
            NORMALIZE("nh", REPLACE_ALL, "ñ");
            NORMALIZE("ess", REPLACE_ALL, "est");
    return normalized_;
}

static std::vector<Word> reorder_helpers(const std::vector<Word>& copy){
    std::vector<Word> sentence_arr = copy;
    vector<Word> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
     INIT_REORDER()

     DEFAULT()

    }

    CLEANUP(reordered_arr);
    
    HANDLE_CASE(&PT_ES_info, NO_CASE, &ptes_case_to)

    
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}

return final_arr;
}


static Word nounLookup(const string& word) {
 
  
LOOKUP(pre, PREPOSITION, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    LOOKUP(pre, PREPOSITION, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    LOOKUP(nouns, NOUN, word, &ptes_gender_from, NO_GENDER, &ptes_plural_from, &ptes_plural_to);
    
    LOOKUP(pro, PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    

     LOOKUP(art, ARTICLE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
    LOOKUP(poss_pro, POSSESSIVE_PRONOUN, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);
  
    LOOKUP(adj, ADJECTIVE, word, &ptes_gender_from,&ptes_gender_to, &ptes_plural_from, &ptes_plural_from);



    LOOKUP(adv, ADJECTIVE, word, NO_GENDER, NO_GENDER, NO_PLURAL, NO_PLURAL);

    
    SUFFIX_LOOKUP(suff, word, adj);

    VERB_LOOKUP(verbs, word, PT_ES_reg, PT_ES_def);


    return { word, word, -1 };
}

MAIN(pt_es, fixed_ngrams, reorder_helpers, nounLookup, false)

#endif