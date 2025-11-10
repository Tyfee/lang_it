// VERY EARLY, dont use this shi 
#include "../lang_it.h"

#if defined(ALL)
#define PT_ES
#endif

#ifdef PT_ES

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;


typedef struct
{
  const char* w;
  const char* t;
  int type;
  int plural;
} Suffix;


typedef struct {
   string word;
   string translation;
   int type;
} Word;

// this will happen at some point
// a final check for anomaly fixing
// it will store different sets of word types like 'i[4] love[3] you[11]' => {4, 3, 11};
// and a normalized frequency number like 0.7, to say whether or not it is valid
typedef struct{
     vector<int> ngram;
     float frequency;
} FrequencyTable;

vector<FrequencyTable> tabela = {
    { {4, 3, 0}, 1.0 },
    { {5, 2, 7}, 0.7 },
    { {4, 4, 3}, 0.3 }
};






Homonym homonimos[1];

const size_t homonimosCount = sizeof(homonimos) / sizeof(homonimos[0]);

// any set of (n)words in portuguese that can't be translated separately

constexpr Entry fixed_ngrams[] = {
  {"bem_vindo", "bienvenido"}
};


template <size_t N>
Suffix lookupSuff(const Suffix (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }

        if (*p == *q) {
            return dict[i];
        }
    }
    return Suffix{nullptr, nullptr, -1, -1};
}

// noun dictionary, not only nouns anymore lol
// basically every word that can't be matched with rules of breakdown will be translated directly from here
constexpr Entry nouns[] = {
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
  {"primeiro", "first"},
  {"segundo", "second"},
  {"terceiro", "third"},
  {"quarto", "fourth"},
  {"número", "number"},
  {"ano", "year"},
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
};

constexpr Entry art[] = {
  {"o", "el"},
  {"a", "la"},
  {"um", "un"},
  {"uma", "una"}
};

constexpr Entry pre[] = { 
  {"do", "del"},
  {"da", "de la"},
  {"ou", "o"},
  {"no", "in the"},
  {"na", "in the"},
  {"nos", "in the"},
  {"nas", "in the"},
  {"à", "to"},
  {"às", "to the"},
  {"ao", "to the"},
   // these verbs are grounded here for misbehaving until second order  
  {"é", "és"},
  {"sou", "soy"},
  {"num", "en un"}
};

// nominative/personal pronouns
constexpr Entry pro[] = {
  {"eu", "yo"},
  {"você", "tu"},
  {"nós",  "we"},
  {"ele",  "él"},
  {"ela",  "ella"},
  {"elas",  "ellas"},
  {"eles", "ellos"}
  
};
constexpr Entry poss_pro[] = {
  {"seu", "tu"},
  {"dela", "su"},
  {"dele",  "su"},
  {"nosso",  "nuestro"},
  {"meu",  "mi"},
  {"meus", "mis"},
  {"minha",  "mi"},
  {"minhas",  "mis"}
};

//object pronoun match (in english)

constexpr Entry obj_pro[] = {
  {"she", "la"},
  {"he", "lo"},
  {"they", "los"},
  {"you", "te"},
};




// oblique pronouns

// adjectives
constexpr Entry adj[] = {
  {"vermelho", "rojo"},
  {"bonito", "belo"},
  {"legal", "genial"},
  {"pequeno", "little"},
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
  {"melhor", "better"},
  {"pior", "worse"},
  {"estranho", "raro"},
  {"esquisito", "raro"},
  {"doente", "enfermo"},
  {"certo", "certain"},
  {"outro", "other"},
  {"gratis", "free"},
  {"livre", "free"},
  {"ultimo", "last"},
  {"doce", "sweet"},
  {"azedo", "sour"},
  {"amargo", "bitter"},
  {"capaz", "capable"},
  {"louco", "crazy"},
  {"próximo", "close"},
  {"perto", "close"},
  {"pesado", "heavy"},
  {"torto", "bent"},
  {"perto", "close"}
};

//adverbs

constexpr Entry adv[] = {
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
  {"ainda", "aún"},
  {"somente", "solamente"},
  {"só", "solo"},
  {"apenas", "solamente"},
  {"então", "entonces"}
};

struct Verb {
    const char* root;       
    const char* translation; 
    bool intransitive;
    bool dative_const;
};
struct VerbEnding {
    const char* ending;
    int code;
};

// verb prefixes where 0 = regular, 1 = irregular conjugation
// is it intransitive?
constexpr Verb reg_verbs[]  = {
  {"fal", "hablar", false, false},
  {"gost", "gustar", false, true},
  {"com", "comer", false, false},
  
};

static const Verb* lookupRegVerb(const char* root) {
    for (const auto& v : reg_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}


constexpr Verb irr_verbs[] = {
  {"fal", "speak", false},
  
};

static const Verb* lookupIrrVerb(const char* root) {
    for (const auto& v : irr_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}



// common suffixes with traceable trnaslation pattern
constexpr Suffix suff[] = {

  {"inho", "ito",0, 0},
  {"inha", "ita", 0, 0},
  {"mento", "ment", 0, 0},
  {"ável", "able", 1, 0},
  {"ível", "ible", 1, 0},
  {"ória", "ory", 0, 0},
  {"cidade", "cidad", 0, 0},
  {"ções", "tions", 0, 1},
  {"tor", "ctor", 0,0},
  {"ais", "ales", 0, 1},
  {"eito", "ecto", 1,0},
  {"ês", "és", 0, 0},
  {"ção", "cion", 0,0},
};



//normalization
//this will turn sets of letters that shift on translation and change them accordingly.
// stuff such as aceitar -> aceipt -> accept
static string normalize(string word) {
    string normalized_ = word;

    if (word.length() > 3) {
        char thirdLast = word[word.size() - 3];
        string last2 = word.substr(word.size() - 2);
       

        
         if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 4) == "agem") {
            normalized_ = normalized_.substr(0, normalized_.size() - 4) + "aje";
        }

        size_t pos = 0;
        while ((pos = normalized_.find("ch", pos)) != string::npos) {
            normalized_.replace(pos, 2, "ll");
            pos += 2; 
        }

        pos = 0;
        while ((pos = normalized_.find("qu", pos)) != string::npos) {
            normalized_.replace(pos, 2, "cu");
            pos += 2; 
        }
        
        pos = 0;
        while ((pos = normalized_.find("lh", pos)) != string::npos) {
            normalized_.replace(pos, 2, "j");
            pos += 2; 
        }
        
        pos = 0;
        while ((pos = normalized_.find("ça", pos)) != string::npos) {
            normalized_.replace(pos, 3, "za");
            pos += 2; 
        }
          pos = 0;
        while ((pos = normalized_.find("nh", pos)) != string::npos) {
            normalized_.replace(pos, 2, "ñ");
            pos += 2; 
        }
         
        if (normalized_.size() >= 3 && (normalized_.substr(normalized_.size() - 2) == "ém" || normalized_.substr(normalized_.size() - 2) == "em")) {
            normalized_ = normalized_.substr(0, normalized_.size() - 2) + "ién";
        }
    }

    return normalized_;
}

static Word morphemeLookup(string word){
  string translation_; 
  string p;
  string m;
  string s;
  int word_type_;

  return Word{word, translation_, word_type_};
};
Word prefixLookup(const std::string &word) {
    using namespace std;
    string translation;

    static const vector<string> infinitive = {"ar", "er", "ir", "ber", "zer"};
    static const vector<string> past_tense_first_sin = {"ei", "ti", "ri", "is", "ia"};
    static const vector<string> past_tense_second_sin = {"iu", "ou", "eu"};

    static const vector<pair<const vector<string>*, int>> groups = {
        {&infinitive, 0},
        {&past_tense_first_sin, 1},
        {&past_tense_second_sin, 2}
    };

    for (auto &[vec, type] : groups) {
        for (const auto &ending : *vec) {
            if (word.size() > ending.size() &&
                word.compare(word.size() - ending.size(), ending.size(), ending) == 0) {

                string base = word.substr(0, word.size() - ending.size());

                // infinitive: just confirm it's a verb form
                if (type == 0) {
                    translation = base  + ending; 
                    return Word{word, translation, 3};
                }

                // past 1st singular
                if (type == 1) {
                    translation = base + "é";
                    return Word{word, translation, 3};
                }

                // past 2nd singular
                if (type == 2) {
                    translation = base + "ó";
                    return Word{word, translation, 3};
                }
            }
        }
    }

    return Word{word, "", -1};
}



static Word suffixLookup(const std::string& word) {
    std::string translation;
    int word_type = 0;

    // example: in- prefix detection
  

    // check suffix patterns
   for (int len = 6; len >= 2; --len) {
    if (word.length() >= static_cast<size_t>(len)) {

        std::string ending = word.substr(word.length() - len);
        Suffix suffResult = lookupSuff(suff, ending.c_str());

        if (suffResult.t) {
            const char* mapped = suffResult.t;
            std::string stem = word.substr(0, word.length() - len);
            word_type = suffResult.type;
            const char* adjResult = lookup(adj, stem.c_str());
            if (adjResult) {
                translation = std::string(adjResult) + mapped;
            word_type = 1;
            }
            else if (!stem.empty()) {
                std::string altStem = stem.substr(0, stem.length() - 1) + "o";
                const char* altAdj = lookup(adj, altStem.c_str());
                if (altAdj)
                    translation = std::string(altAdj) + mapped;
                else
                    translation = stem + mapped;
            }
            else {
                translation = stem + mapped;
            }

            return {word, normalize(translation), word_type};
        }
    }
}


    return {word, word, -1}; // fallback: unchanged
}



static Word nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type = -1;


  
  
  // rules
   bool plural = ((lookup(nouns, (word.substr(0, word.length() - 2)).c_str()) || lookup(nouns, (word.substr(0, word.length() - 1)).c_str()) || lookup(nouns, (word.substr(0, word.length() - 2) + "o").c_str())) && word.substr(word.length() - 1) == "s"); // this is plural nouns only
   bool gender_shift = lookup(nouns, (word.substr(0, word.length() - 1)  + "o").c_str()); // this is gender shift for nouns only


   bool adj_plural = ((lookup(adj ,(word.substr(0, word.length() - 1).c_str())) || lookup(adj, (word.substr(0, word.length() - 2) + "o").c_str())) && word.substr(word.length() - 1) == "s"); // this is plural adjectives only
   bool adj_gender_shift = lookup(adj, (word.substr(0, word.length() - 1)  + "o").c_str()); // this is gender shift for adjectives only
   bool article_plural = lookup(art, (word.substr(0, word.length() - 1).c_str()));
  

  // for each individual word loop, you look in the noun dictionary
  //first with accentuation, 
  if(lookup(nouns, word.c_str())){
   translation = lookup(nouns, word.c_str());
   word_type = 0;
   }
   //then without accentuation (helpful in plural)
   else if(lookup(nouns, word.c_str())){
       translation = lookup(nouns, word.c_str());
       word_type = 0; 
   }
   else if(lookup(adj, word.c_str())){
    
      translation = lookup(adj, word.c_str());
      word_type = 1;

    }
    else if(lookup(pro, word.c_str())){
      translation = lookup(pro, word.c_str());
      word_type = 4;
    } else if(lookup(poss_pro, word.c_str())){
      translation = lookup(poss_pro, word.c_str());
      word_type = 40;
    }
  

     else if(lookup(pre, word.c_str())){
      translation = lookup(pre, word.c_str());
      word_type = 8;
    }//preposition plurals are simple i guess
    else if(lookup(pre, word.substr(0, word.length() - 1).c_str())){
      translation = lookup(pre, word.substr(0, word.length() - 1).c_str());
      word_type = 8;
    }
     else if(lookup(art, word.c_str())){
      translation = lookup(art, word.c_str());
      word_type = 9;
    }
     else if(article_plural){
      translation = lookup(art, (word.substr(0, word.length() - 1).c_str()));
      word_type = 9;
    }
    
     else if(lookup(adv, word.c_str())){
      translation = lookup(adv, word.c_str());
      word_type = 13;
    }
    
    
  else if(plural){
    // by removing the last letter of the word, we can check for **BASIC** plural. e.g casa[s] -> casa
    //if the noun ends in f or fe, we substitute for ves, life -> lives, leaf => leaves
    string singular_pt;
    string word_normalized = word;

    if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "os") {
        singular_pt = word.substr(0, word.size() - 2) + "o"; // gatos -> gato, cachorros -> cachorro
    } else if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "as") {
        singular_pt = word.substr(0, word.size() - 2) + "a"; // casas -> casa, cachorras -> cachorra, crianças -> criança
               std::string key = singular_pt.substr(0, singular_pt.size() - 1) + "o";
                    if(lookup(nouns, key.c_str())){
                        singular_pt = key; 
                    }
    } else if (word_normalized.size() > 2 && word_normalized.substr(word_normalized.size() - 2) == "es") {
        singular_pt = word.substr(0, word.size() - 2);       // flores -> flor
    } else if (word_normalized.size() > 1 && word_normalized.back() == 's') {
        singular_pt = word.substr(0, word.size() - 1);       // fallback para palavras terminadas em s
    } else {
        singular_pt = ""; // não é plural conhecido
    }

    if (lookup(nouns, singular_pt.c_str())) {
        string singular_en = lookup(nouns, singular_pt.c_str());
        if (!singular_en.empty()) {
            if (singular_en.size() >= 2 && singular_en.substr(singular_en.size()-2) == "fe") {
                translation = singular_en.substr(0, singular_en.size()-2) + "ves";
            } else if (singular_en.back() == 'f') {
                translation = singular_en.substr(0, singular_en.size()-1) + "ves";
            } else {
                translation = singular_en + "s";
            }
            word_type = 0;
            translation = normalize(translation);
        }
    }
}

    // same as above for adjectives. e.g bonito[s] -> bonito, except we dont plug in 's' cause english has no adj. plurals ;p
      else if(adj_plural){

        if(lookup(adj, (word.substr(0, word.length() - 1).c_str()))){
         translation = lookup(adj, (word.substr(0, word.length() - 1).c_str()));

        }
        
        else if(lookup(adj,(word.substr(0, word.length() - 2) + "o").c_str())){
             translation = lookup(adj,(word.substr(0, word.length() - 2) + "o").c_str());
          }

      word_type = 1;
    }  
    // by switching the last letter of the word, we can check for **BASIC** gender shift. e.g cachorra -> (cachorra - a) + o -> cachorro 
   else if(gender_shift){
          
         translation = lookup(nouns, (word.substr(0, word.length() - 1) + "o").c_str());
         word_type = 0;
        }
        
    // same as above for adjectives. e.g pequena -> (pequena - a) + o -> pequeno
    else if(adj_gender_shift){

         translation = lookup(adj,(word.substr(0, word.length() - 1) + "o").c_str());
         word_type = 1;
        }
    
         // if not found morpheme
        else if(morphemeLookup(word).translation.length() > 0){
        // if suffix not found, look for morpheme breakdown
        translation = morphemeLookup(word).translation;
        word_type = morphemeLookup(word).type;
        
      }
   
        // if preffix not found, look for prefix
       else if(prefixLookup(word).translation.length() > 0){
        translation = prefixLookup(word).translation;
        word_type = prefixLookup(word).type;
       }
       // if not found suffix match
        else if(suffixLookup(word).translation.length() > 0 || suffixLookup(word.substr(0, word.length() - 1)).translation.length() > 0){     
           string look = suffixLookup(word).translation.length() > 0 ? suffixLookup(word).translation : suffixLookup(word.substr(0, word.length() - 1)).translation + "s";
        translation = look;
        word_type = suffixLookup(word).translation.length() > 0 ? suffixLookup(word).type : suffixLookup(word.substr(0, word.length() - 1)).type;
       }
   
       else{
           return {word, word, -1};
      }
  return {word, normalize(translation), word_type};
}



// when words need to switch order
// this is actually various manipulations (Take Off The Blindfold REFERENCE????)
//not only word order, but i'm not changing the name at this point
static vector<Word> reorder_helpers(vector<Word> sentence_arr) {
    
    vector<Word> reordered_arr;
for (size_t j = 0; j < sentence_arr.size(); ++j) {
}

    for (size_t i = 0; i < sentence_arr.size(); ++i) {


        // ------------------------ ADJ + VERBS ----------------- 
        // yk like, díficil de jogar -> very hard of to play . you cant have an adjective and a verb with a broken connector (8)
            if (i > 0 && sentence_arr[i - 2].type == 1 && sentence_arr[i - 1].type == 8  &&  (sentence_arr[i].type == 3 || sentence_arr[i].type == 36)) {
            if (sentence_arr[i - 1].translation == "don't" || sentence_arr[i - 1].translation == "doesn't") {
                reordered_arr.push_back(Word{ sentence_arr[i].word, sentence_arr[i].translation, sentence_arr[i].type});
                continue;
            }
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{ sentence_arr[i - 1].word, sentence_arr[i - 1].translation, sentence_arr[i].type});
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"de", "to", -1}); 
            reordered_arr.push_back(Word{ sentence_arr[i].word, sentence_arr[i].translation, sentence_arr[i].type});
          
    
    } 

  

else if (i > 0 && (sentence_arr[i - 1].type == 8 || sentence_arr[i - 1].type == 13) && (sentence_arr[i].type == 3 || sentence_arr[i].type == 36 )) {

 
}
  // ------------------------ ARTICLE VS PREPOSITION (e.g A VS À) COMPETITION  --------------------
        // when a match is 'a', is the next word a pronoun[4] or an article[9]? that means its "à" if not "a"
        // since it defaults to 'the' i dont think i need a fallback 
        else if (i > 0 && sentence_arr[i - 1].translation == "the" && (sentence_arr[i].type == 4 || sentence_arr[i].type == 9)) {
          reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{"à","to", 20});
            reordered_arr.push_back(Word{sentence_arr[i].word, sentence_arr[i].translation, sentence_arr[i].type});  
        } 
        


        else if (i > 1 && (sentence_arr[i - 2].type == 3 || sentence_arr[i - 2].type == 3) && sentence_arr[i - 1].translation == "of" && (sentence_arr[i].type == 3 || sentence_arr[i].type == 36)) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(Word{sentence_arr[i - 2].word, sentence_arr[i - 2].translation, sentence_arr[i - 2].type});  
            reordered_arr.push_back(Word{"de", "to", -1});  
            reordered_arr.push_back(Word{ sentence_arr[i].word, sentence_arr[i].translation, sentence_arr[i].type});
        } 

      

        else {
            if (sentence_arr[i].type != -1)  // only push if not processed
                reordered_arr.push_back(Word{ sentence_arr[i].word, sentence_arr[i].translation, sentence_arr[i].type});
        }
    }

    // this guy will do word manipulation, the above one will take the name literally (reorder helpers)
    // needed to do this cause handling it all on the same iteration on the original array was impossible    

     for (size_t i = 0; i < reordered_arr.size(); ++i) {
    if (reordered_arr[i].translation == "is") {
        
        bool preceded_by_pronoun = (i > 0 &&
            (reordered_arr[i-1].type == 4 || reordered_arr[i-1].type == 0 || reordered_arr[i-1].type == 13 || reordered_arr[i-1].type == 2));

        if (preceded_by_pronoun) {
            reordered_arr[i].translation = "is";
        } else {
            reordered_arr.push_back(Word{"$it", "it", 4 });
            i++; 
        }
    }  


}

// interrogative
if(sentence_arr.size() > 0){
    string to_push;
    string last = sentence_arr[sentence_arr.size() - 1].word;
    //　QUESTION　QUESTION　僕は　QUESTION　QUESTION　いったい　QUESTION　QUESTION　君の何を知っていたの????????????
    if(last == "?"){
        to_push = "¿";
    }else if(last == "!"){
        to_push = "¡";
    }
        reordered_arr.clear();
        reordered_arr.push_back(Word{to_push, to_push, -2});
        for(int i = 0; i < sentence_arr.size(); ++i){
              reordered_arr.push_back(sentence_arr[i]);
        }
    }



//statistics layer
vector<Word> final_arr;
for (size_t i = 0; i < reordered_arr.size(); ++i) {
    final_arr.push_back(reordered_arr[i]);
}
//homonym mediation
// need to remember what portuguese homonyms arent homonyms in spanish
for (size_t i = 0; i < final_arr.size(); ++i) {
    if (final_arr[i].word == "manga") {
        
        int start = max(0, static_cast<int>(i) - 2);
        int end   = min(static_cast<int>(final_arr.size()) - 1, static_cast<int>(i) + 2);

        vector<string> context;
        for (int j = start; j <= end; ++j) {
            context.push_back(final_arr[j].translation);
        }
        
        size_t contextIndex = static_cast<size_t>(i - start);

        vector<string> spanish_context = context;
        vector<int> word_types;
        spanish_context[contextIndex] = final_arr[i].word;  
        word_types[contextIndex] = final_arr[i].type;  
        
        string resolved_word = semantics(spanish_context, word_types,contextIndex, homonyms, homonymCount);

        final_arr[i].translation = resolved_word;
    }
}


    return final_arr;
}


//ngram groups
static std::string unigramLookup(vector<string> array_of_words, vector<int> ignore_flags){

  vector<Word> sentence_arr;
  vector<Word> word_arr;

  int match_type;
  string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    
    Word match = nounLookup(array_of_words[i]);
    switch (ignore_flags[i])
    {
    case 0:{
    match_type = match.type;
    if(match.type == -1) match_type = 0;
    
         Word match_ = {array_of_words[i], match.translation, match_type};
        sentence_arr.push_back({match.word, match.translation ,match_type});
        word_arr.push_back(match_);
        break;}
    case 1:{
        Word match_ = {array_of_words[i], array_of_words[i], 0};
       sentence_arr.push_back({array_of_words[i], array_of_words[i],0});
         word_arr.push_back(match_);
       break;}
    default:
      break;
    }
  }
  sentence_arr = reorder_helpers(word_arr);

  
 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const std::string& token = sentence_arr[i].translation;

    char firstChar = token.empty() ? '\0' : token[0];
    bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                          firstChar == '.' || firstChar == ','
                          || firstChar == '-' || firstChar == '/' || firstChar == ':');

    if (!sentence.empty() && !isPunctuation) {
        sentence += " ";
    }

    sentence += token;
}
  return sentence;
}

static std::string bigramLookup(const std::vector<std::string>& words, std::vector<int>& ignore_flags) {
    std::vector<std::string> mended_array_of_words;
    std::vector<int> new_ignore_flags;

    size_t i = 0;
    while (i < words.size()) {
        if (i + 1 < words.size() && ignore_flags[i] == 0 && ignore_flags[i + 1] == 0) {
            std::string bigram = words[i] + "_" + words[i + 1];
            const char* bigram_translation = lookup(fixed_ngrams, bigram.c_str());
            
            if (bigram_translation) {
                mended_array_of_words.push_back(bigram_translation);
                new_ignore_flags.push_back(1);  
                i += 2;  
                continue;
            }
        }
        
        mended_array_of_words.push_back(words[i]);
        new_ignore_flags.push_back(ignore_flags[i]);
        i++;
    }

    return unigramLookup(mended_array_of_words, new_ignore_flags);
}

static std::string trigramLookup(const std::vector<std::string>& words) {
    std::vector<std::string> mended;
    std::vector<int> ignore_flags(words.size(), 0);

    size_t i = 0;
    while (i < words.size()) {
        if (i + 2 < words.size()) {
            std::string trigram = words[i] + "_" + words[i + 1] + "_" + words[i + 2];
            const char* trigram_translation = lookup(fixed_ngrams, trigram.c_str());
            
            if (trigram_translation) {
                mended.push_back(trigram_translation);
                ignore_flags.push_back(1);  
                i += 3;  
                continue;
            }
        }
        mended.push_back(words[i]);
        ignore_flags.push_back(0);
        i++;
    }

    // Then process bigrams on the result
    return bigramLookup(mended, ignore_flags);
}

std::string traduzir_es(const char* sentence) {
    char buffer[250];
    strncpy(buffer, sentence, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    to_lower(buffer);
    vector<string> arr = tokenize(string(buffer));  
    std::string translated = trigramLookup(arr);
    
    return translated; 
}

#endif