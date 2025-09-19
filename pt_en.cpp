#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>
#include <sstream> 
#include "lang_it.h"

using namespace std;


typedef struct
{
  const char* w;
  const char* t;
} Entry;

// constexpr struct test, eventually i want this bitch to be embedded-friendly; so i'll try to eliminate
// maps, then vectors, then strings (highly doubt i'm capable but eh)
// any set of (n)words in portuguese that can't be translated separately

constexpr Entry fixed_ngrams[] = {
  {"de_novo", "again"},
  {"o_que", "what"},
  {"por_que", "why"},
  {"do_que", "than"},
  {"por_favor", "please"},
  {"por_causa", "because of"}
};

template <size_t N>
const char* lookup(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].t;
    }
    return nullptr;
}


// noun dictionary, not only nouns anymore lol
// basically every word that can't be matched with rules of breakdown will be translated directly from here
map<string, string> nouns = {
  {"melancia", "watermelon"},
  {"areia", "sand"},
  {"chuva", "rain"},
  {"açucar", "sugar"},
  {"cachorro", "dog"},
  {"água", "water"},
  {"suco", "juice"},
  {"laranja", "orange"}, // how the hell will i handle that ?
  {"e", "and"},
  {"porta", "door"},
  {"janela", "window"},
  {"não", "no"}, // ts is dynamic as well, not before nouns and 'don't' before verbs
  {"jogo", "game"}, // TODO, differentiate a noun vs the 1st person singular (um jogo vs eu jogo)
  {"todo", "all"},
  {"comida", "food"},
  {"cidade", "town"},
  {"arma", "gun"},
  {"vida", "life"},
  {"folha", "leaf"},
  {"faca", "knife"},
  {"gato", "cat"},
  {"mulher", "woman"},
  {"homem", "man"},
  {"pessoa", "person"},
  {"cogumelo", "mushroom"},
  {"nuvem", "cloud"}, // TODO. IRREGULAR PLURAL SUCH AS M => NS
  {"flor", "flower"},
  {"dinheiro", "money"}
};

map <string, string> art = {
  {"o", "the"},
  {"a", "the"},
  {"um", "a"},
  {"uma", "a"},
};

constexpr Entry pre[] = { 
  {"do", "of"},
  {"da", "of"},
  {"de", "of"},
  {"com", "with"}, // have to differentiate this from 'como' as in 'like' 
  {"sem", "without"},
  {"ou", "or"},
  {"em", "in"}
};

// nominative/personal pronouns
constexpr Entry pro[] = {
  {"eu", "i"},
  {"voce", "you"},
  {"nos",  "we"},
  {"ele",  "he"},
  {"ela",  "she"},
  {"elas",  "they"},
  {"eles", "they"}
};
constexpr Entry poss_pro[] = {
  {"seu", "your"},
  {"dela", "her"},
  {"dele",  "his"},
  {"nosso",  "our"},
  {"meu",  "my"}
};

//object pronoun match (in english)

map<string, string> obj_pro = {
  {"she", "her"},
  {"he", "him"},
  {"they", "them"},
};



vector<string> th_per_aux = {"she", "he", "it"};
vector<string> reg_aux = {"i", "you", "we", "they"};

// oblique pronouns
map<string, string> obl_pro = {
  {"te", "you"},
  {"me", "me"},
};

// adjectives
map<string, string> adj = {
  {"azul", "blue"},
  {"vermelho", "red"},
  {"bonito", "beautiful"},
  {"legal", "cool"},
  {"grande", "big"},
  {"forte", "strong"},
  {"fraco", "weak"},
  {"pequeno", "little"},
  {"grande", "big"},
  {"mais", "more"},
  {"engraçado", "funny"},
  {"molhado", "wet"},
  {"seco", "dry"},
  {"novo", "new"},
  {"triste", "sad"},
  {"feliz", "happy"},
  {"alto", "high"}
};

//adverbs

constexpr Entry adv[] = {
  {"mas", "but"},
  {"quando", "when"},
  {"quem", "who"},
  {"se", "if"},
  {"também", "too"},
  {"porque", "because"},
  {"onde", "where"},
  {"quantos", "how many"},
  {"quanto", "how much"},
  {"nunca", "never"},
  {"sempre", "always"},
  {"aqui", "here"},
  {"ali", "there"},
  {"desde", "since"},
  {"ninguém", "nobody"}
};

struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
};

// verb prefixes where 0 = regular, 1 = irregular conjugation
constexpr Verb reg_verbs[]  = {
  {"am", "lov", 0},
  {"gost", "lik", 0},
  {"quer", "want", 1},
  {"quis", "want", 1},
  {"corr", "run", 1},
  {"abr", "open", 1},
  {"fech", "clos", 0},
  {"pergunt", "ask", 1},
  {"pe", "ask", 1},
  {"precis", "need", 1},
  {"morr", "di", 0},
  {"sonh", "dream", 1},
  {"grit", "scream", 1},
  {"acredit","believ", 0},
  {"viv", "liv", 0},
  {"tent", "try", 1}
};

const Verb* lookupRegVerb(const char* root) {
    for (const auto& v : reg_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}


map<string, pair<string, int>> irr_verbs = {
  {"fal", {"speak", 1}},
  {"beb", {"drink", 1}},
  {"sangr", {"bleed", 1}},
  {"procri", {"breed", 1}},
  {"aliment", {"feed", 1}},
  {"nad", {"swim", 1}},
  {"quebr", {"break", 1}},
  {"escrev", {"writ", 1}},
  {"dirig", {"driv", 0}},
  {"est", {"%", 1}}, // % is a flag for the 'to be' verb, i don't want to figure out a clever way to do that right now, so i'll simply mark it with a flag
                    // and deal with conjugation based on what's goin on around it (i - 1) and (i + 1), since it could match:
                    // está (3rd person singular => is) estamos (1st person plural => are) estão => (3rd person plural => are) or estou (1st person singular => am)

  {"cant", {"sing", 1}},
  {"ganh", {"win", 1}},
  {"é", {"is", 1}},
  {"são", {"are", 1}},
  {"sou", {"am", 1}},
  {"volt", {"go back", 1}},
  {"te", {"have", 1}},
  {"com", {"eat", 1}},
  {"lut", {"fight", 1}},
  {"pod", {"can", 1}},
  {"pos", {"can", 1}},
  {"dev", {"should", 1}}
};

// 0 == pt 1 = ode 2 = ct 3 == ate 4 == ed
// quick dirty verb guessing
map <string, int> patt_verbs = {
  {"ptar", 0}, // adaptar = adapt,
  {"odir", 1}, // explodir = explode 
  {"tar", 2}, // contatar = contact
  {"trair", 2}, // extrair = extract
  {"trar", 3}, // contatar = contact // what about encontrar.... (needs more specification)
  
  //surely theres a way to imolement the past tense endings dinamically, will i? who knows
  {"ptou", 4}, {"ptei", 4},
  {"odiu", 4}, {"odi", 4},
  {"tou", 4}, {"tei", 4},
  {"traiu", 4}, {"traí", 4},
  {"trou", 4}, {"trei", 4},
};


// common suffixes with traceable trnaslation pattern
constexpr Entry suff[] = {
  {"mente", "ly"},
  {"ável", "able"},
  {"ível", "ible"},
  {"ória", "ory"},
  {"ência", "ency"},
  {"cidade", "city"},
  {"açado", "aced"},
  {"ágico", "agic"},
  {"ágica", "agic"},
  {"culo", "cle"},
  {"cula", "cle"},
  {"cleta", "cle"}
};

//is it a vowel? 
bool isVowel(char x)
{
    if (x == 'a' || x == 'e' || x == 'i' || x == 'o'
        || x == 'u' || x == 'A' || x == 'E' || x == 'I'
        || x == 'O' || x == 'U')
    return true;
    else
     return false;
}

// dictionary lookup

//look for preffix matches 
pair<string, int> prefixLookup(string word){

  
  string translation = word; 
  int word_type;

  vector<string> infinitive = {"ar", "er", "ir", "dir", "r"};
  vector<string> present_non_s = {"o", "to", "go", "ro", "am", "em", "amos", "mo", "lo", "ço", "nho", "so"};
  vector<string> present_s = {"a", "ta", "re", "ga"};
  vector<string> general_past = {"ei", "ou", "eu", "ti", "aram", "ia", "ri", "i", "iu"};
  vector<string> present_continuous = {"ando"};
  vector<string> completed_past = {"ava", "ávamos"};
  vector<string> subjunctive = {"sse", "ssemos"};
  vector<string> conditional = {"aria", "ariamos", "eria"};


  // this will try to find a verb ending that can be translated to past tense or infinitive or continuous or whatever
  // it's a lambda that returns a pair with the match lemma + the vowel/conjugation.
auto find_verb = [](vector<string> format, string word, int verb_info) {

  for(size_t i = 0; i < format.size(); ++i){

    
     string translation_;
     int word_type_;
     int verb_type;
     bool aux = false;
     string ending;

     
      size_t match = word.rfind(format[i]);
      if (match != string::npos && match + format[i].length() == word.length()) {
       string root = word.substr(0, match);
      string ending_ = "";

char buffer[64];

const Verb* v = lookupRegVerb(root.c_str());

if (v) {
    const char* ending = "";
    switch (verb_info) {
        case 0: ending = (v->type == 0) ? "e" : ""; break;
        case 1: ending = "ed"; break;
        case 2: ending = (v->type == 0) ? "e" : ""; break;
        case 3: ending = (v->type == 0) ? "es" : "s"; break;
        case 4: ending = "e"; break;
        default: break;
    }

    if (verb_info == 4) { 
        const char* used = "used to ";
        size_t i = 0;
        while (*used && i + 1 < sizeof(buffer)) buffer[i++] = *used++;
        const char* base = v->translation;
        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
        buffer[i++] = 'e'; // final 'e'
        buffer[i] = '\0';
    }
    else if(verb_info == 6){
       const char* would = "would ";
        size_t i = 0;
        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
        const char* base = v->translation;
        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
        buffer[i++] = 'e'; // final 'e'
        buffer[i] = '\0';
    }
    else{
       size_t i = 0;
        const char* base = v->translation;
        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
        const char* e = ending;
        while (*e && i + 1 < sizeof(buffer)) buffer[i++] = *e++;
        buffer[i] = '\0';
    }

    word_type_ = 3;
    verb_type = v->type;

   return pair<string, int>{string(buffer), word_type_ };

}
       if(irr_verbs.find(root) != irr_verbs.end()){
        
        if(root == "est"){
          // estou == am estamos == are está == is estão == are
          if (word.substr(3, word.length()) == "ou"){
           translation_ = "am";
          }
          else if(word.substr(3, word.length()) == "amos" || word.substr(3, word.length()) == "ão"){
            translation_ = "are";
          }
            else if(word.substr(3, word.length()) == "á"){
            translation_ = "is";
          }
           
           word_type_ = 28; // 28 == to be cause 2 == TO 8 == B
           return pair<string, int>{translation_, word_type_};
       }

        switch (verb_info)
         {
            case 0:
                if(irr_verbs[root].second == 0){
                  ending = "e";
                }else{
                  ending = "";
                }
            break;
            case 1:
             ending = "ed";
            break;
            case 2:
               if(irr_verbs[root].second == 0){
                  ending = "e";
                }else{
                  ending = "";
                }
            break;
            case 3:
               if(irr_verbs[root].second == 0){
                  ending = "es";
                }else{
                  ending = "s";
                }
            break;
            case 4:
             ending = "e";
            break;
             case 5:
             ending = "ing";
            break;
             case 6:
              if(irr_verbs[root].second == 0){
                  ending = "e";
                }else{
                  ending = "";
                }
            break;
            default:
            break;
        }; 
         if(verb_info == 4){
            
              translation_ = "used to " + irr_verbs[root].first + ending;
           }else if(verb_info == 6){
              if (root == "pod"){
                translation_ = "could";
               } else if(root == "dev"){
                 translation_ = "should";
               }else{
              translation_ = "would " + irr_verbs[root].first + ending;
              }
              aux = true;
           }
           else if(verb_info == 1){
         //TODO: Set up the very specific rules that most verbs can abide to.
                
                if(irr_verbs[root].first.substr(irr_verbs[root].first.length() - 3, 3) == "eed"){
                      translation_ = irr_verbs[root].first.substr(0, irr_verbs[root].first.length() - 3) + "ed";

                      // this is a weird ass pattern that works for a small lil list of verbs (7 as of right now T-T)
                      // if an irregular verb starts with two consonants (substr(0, 1) and substr(1, 1) dont pass isVowel())
                      // and the two consonants are followed by either 'ea', 'i' or 'ee'*
                      // BUT IT DOESNT END IN D, G, P, K or M LMAOOOOOOOOOO
                      // you basically get the vowel(s)* that follow the two consonants and replace them with an O
                    //and if it DOESNT end with an E, you add it 
                        // the vowel to o shift also works if the verb ends in 'get' (get => got, forget => forgot )
                      // this way steal => stole, break => broke, speak => spoke, drive => drove 

                } else if (irr_verbs[root].first.length() >= 3 && // is the word more than three letters?
                      !isVowel(irr_verbs[root].first[0]) &&   // is the first letter not a vowel?
                      !isVowel(irr_verbs[root].first[1]) &&   // is the second letter not a vowel?
                      (irr_verbs[root].first.substr(2, 2) == "ea" || //are they followed by either "ea" || "i" || "ee"
                        irr_verbs[root].first.substr(2, 1) == "i" || 
                        irr_verbs[root].first.substr(2, 2) == "ee") &&
                      irr_verbs[root].first.back() != 'd' &&  // do they NOT end in 'd' || 'g' || 'p' || 'k'
                      irr_verbs[root].first.back() != 'g' && 
                      irr_verbs[root].first.back() != 'p' &&
                     irr_verbs[root].first.back() != 'k' &&
                       irr_verbs[root].first.back() != 'm'
                    ) {
                            size_t pos;
                            size_t length;

                            // qual das vogais é a que o verbo tem? 
                            if ((pos = irr_verbs[root].first.find("ea")) != string::npos) {
                                length = 2;  
                            } else if ((pos = irr_verbs[root].first.find("i")) != string::npos) {
                                length = 1; 
                            } else if ((pos = irr_verbs[root].first.find("ee")) != string::npos) {
                                length = 2; 
                            } else {
                                translation_ = irr_verbs[root].first;
                            }

                          translation_ = irr_verbs[root].first;
                          
                          // remover as vogais e substituir por 'o'
                          translation_.replace(pos, length, "o");
                          if (translation_.back() != 'e') {
                              translation_ += "e";
                          }
                else{
                  translation_ = irr_verbs[root].first;
                }
            }   
            // ANOTHER HYPERSPECIFIC RULE
            // is the verbs second letter not 'h'?
            // does it end in either ng or nk?

            string base_ = irr_verbs[root].first;
            if (base_.length() >= 3 
            && base_[1] != 'h' 
           && (base_.substr(base_.length()-2) == "ng" 
           || base_.substr(base_.length()-2) == "nk"))
            {
                          // replace the i for an 'a' so that e.g: drink => drank
                translation_ = base_.replace(base_.find("i"), 1, "a");;  
            }
          }
           else{
            translation_ = irr_verbs[root].first + ending;
       
           }
        
           word_type_ = !aux ? 3 : 33;
           verb_type = irr_verbs[root].second;
           cout << verb_type;

           return pair<string, int>{translation_, word_type_};
       }
              // verb guesser, only for infinitive, will figure out how to add the other tenses 
              // i think every [latin-origin] verb that fits this type of convertion is ALWAYS regular
              // will check for exceptions later
       for (const auto& [endingStr, code] : patt_verbs) {
        if (word.size() >= endingStr.size() &&
            word.compare(word.size() - endingStr.size(), endingStr.size(), endingStr) == 0) {
            
           
            
            string stem;
            
            if(code == 4){ 
               stem = word.substr(0, word.size() -( endingStr.size() - 2));
            }else{
              stem = word.substr(0, word.size() - endingStr.size());
            }
            switch (code) {
                case 0: ending = "pt";   break;
                case 1: ending = "ode";  break;
                case 2: ending = "ct";   break;
                case 3: ending = "trate";   break;
                case 4: ending = "ed";   break;
            }
            
            translation_ = stem + ending;
            word_type_ = 3;
            return pair<string, int>{translation_, word_type_};
        }
        }
     }
  }
      return pair<string, int>{"", -1};
};
// try with every possible ending set, infinitive being the first match attempt
auto result_set = find_verb(infinitive, word, 0);

if (result_set.first.empty()) 
    result_set = find_verb(general_past, word, 1);

if (result_set.first.empty()) 
    result_set = find_verb(present_non_s, word, 2);

if (result_set.first.empty()) 
    result_set = find_verb(present_s, word, 3);

if (result_set.first.empty()) 
    result_set = find_verb(completed_past, word, 4);

if (result_set.first.empty()) 
    result_set = find_verb(present_continuous, word, 5);

if (result_set.first.empty()) 
    result_set = find_verb(conditional, word, 6);


if (result_set.first.empty()) 
    result_set = pair<string, int>{irr_verbs[word].first, 3};

  return result_set;
}

//look for suffix matches
pair<string, int> suffixLookup(string word){
  
 string translation;
 int word_type;
 // TODO, make this smaller and not manually check if

 // if the last 6 letters of a word match a suff. e.g: totalmente, strip the 6 letters and look up
 // the corresposing translation for the length-6 suffix

 // i also need to figure out how to handle stuff like incredibly
 if(word.length() > 6){
  if(lookup(suff, word.substr(6).c_str())){
    if(adj.count(word.substr(0, 6))){
      translation = adj[word.substr(0, 6)] + lookup(suff, word.substr(6).c_str());
    }else{
      translation = word.substr(0, 6) + lookup(suff, word.substr(6).c_str());
    }
    
  word_type = 2;
  }
  //same shit 
  else if(lookup(suff, word.substr(5).c_str())){
      if(adj.count(word.substr(0, 5))){
      translation = adj[word.substr(0, 5)] + lookup(suff, word.substr(5).c_str());
     }else{
      translation = word.substr(0, 5) + lookup(suff, word.substr(5).c_str());
     }
  }
  //same shit 
  else if(lookup(suff, word.substr(4).c_str())){
     if(adj.count(word.substr(0, 4))){
      translation = adj[word.substr(0, 4)] + lookup(suff, word.substr(4).c_str());
     }else{
      translation = word.substr(0,4) + lookup(suff, word.substr(4).c_str());
     }
  }
  else if(lookup(suff, word.substr(3).c_str())){
       if(adj.count(word.substr(0, 3))){
      translation = adj[word.substr(0, 3)] + word.substr(3).c_str();
     }else{
      translation = word.substr(0, 3) + lookup(suff, word.substr(3).c_str());
     }
  }
    else if(lookup(suff, word.substr(2).c_str())){
    translation = word.substr(0, 2) + lookup(suff, word.substr(2).c_str());
  }
    else{
        translation = "";  
        word_type = -1;
    }
}
  return {translation, word_type};
}

pair<string, int> nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type;
  
  // rules
   bool plural = ((nouns.count(word.substr(0, word.length() - 1)) || nouns.count(word.substr(0, word.length() - 2) + "o")) && word.substr(word.length() - 1) == "s"); // this is plural nouns only
   bool gender_shift = nouns.count(word.substr(0, word.length() - 1)  + "o"); // this is gender shift for nouns only
   bool diminutive = nouns.count(word.substr(0, word.length() - 4) + "o") ||  nouns.count(word.substr(0, word.length() - 6) + "o");
   bool adj_plural = ((adj.count(word.substr(0, word.length() - 1)) || adj.count(word.substr(0, word.length() - 2) + "o")) && word.substr(word.length() - 1) == "s"); // this is plural adjectives only
   bool adj_gender_shift = adj.count(word.substr(0, word.length() - 1)  + "o"); // this is gender shift for adjectives only
   bool article_plural = art.count(word.substr(0, word.length() - 1));
  

  // for each individual word loop, you look in the noun dictionary
   if(nouns.count(word)){
   translation = nouns[word];
   word_type = 0;
   }
   else if(adj.count(word)){
      translation = adj[word];
      word_type = 1;
    }
    else if(lookup(pro, word.c_str())){
      translation = lookup(pro, word.c_str());
      word_type = 4;
    } else if(lookup(poss_pro, word.c_str())){
      translation = lookup(poss_pro, word.c_str());
      word_type = 4;
    }
  
    else if(obl_pro.count(word)){
      translation = obl_pro[word];
      word_type = 11;
    }

     else if(art.count(word)){
      translation = art[word];
      word_type = 9;
    }
     else if(article_plural){
      translation = art[word.substr(0, word.length() - 1)];
      word_type = 9;
    }
     else if(lookup(pre, word.c_str())){
      translation = lookup(pre, word.c_str());
      word_type = 8;
    }
    
     else if(lookup(adv, word.c_str())){
      translation = lookup(adv, word.c_str());
      word_type = 13;
    }
    
   else if(plural){
    // by removing the last letter of the word, we can check for **BASIC** plural. e.g casa[s] -> casa
    //if the noun ends in f or fe, we substitute for ves, life -> lives, leaf => leaves
    string singular_pt;
if (word.size() > 2 && word.substr(word.size() - 2) == "os" && nouns.count(word.substr(0, word.size() - 2) + "o")) {
    singular_pt = word.substr(0, word.size() - 2); // "gatos" -> "gato"
} else if (word.size() > 2 && word.substr(word.size() - 2) == "as" && nouns.count(word.substr(0, word.size() - 2) + "o")) {
    singular_pt = word.substr(0, word.size() - 2); // "gatas" -> "gato"
} else if (word.size() > 1 && word.substr(word.size() - 1) == "s" && nouns.count(word.substr(0, word.size() - 1))) {
    singular_pt = word.substr(0, word.size() - 1); // "casas" -> "casa"
} else {
    singular_pt = ""; // não é plural conhecido
}

if(!singular_pt.empty()) {
    string singular_en = nouns[singular_pt + (word.substr(word.size()-2)=="as" || word.substr(word.size()-2)=="os" ? "o" : "")];
    translation = (!singular_en.empty() && singular_en.size() >= 2 && singular_en.substr(singular_en.size()-2) == "fe")
        ? singular_en.substr(0, singular_en.size()-2) + "ves"
        : (singular_en.back() == 'f'
            ? singular_en.substr(0, singular_en.size()-1) + "ves"
            : singular_en + "s");
    word_type = 0;
}

}
    // same as above for adjectives. e.g bonito[s] -> bonito, except we dont plug in 's' cause english has no adj. plurals ;p
      else if(adj_plural){
        if(adj.count(word.substr(0, word.length() - 1))){
         translation = adj[word.substr(0, word.length() - 1)];

        }else if(adj.count(word.substr(0, word.length() - 2) + "o")){
             translation = adj[word.substr(0, word.length() - 2) + "o"];
          }

      word_type = 1;
    }  
    // by switching the last letter of the word, we can check for **BASIC** gender shift. e.g cachorra -> (cachorra - a) + o -> cachorro 
   else if(gender_shift){
          
         translation = nouns[word.substr(0, word.length() - 1) + "o"];
         word_type = 0;
        }
        
    // same as above for adjectives. e.g pequena -> (pequena - a) + o -> pequeno
    else if(adj_gender_shift){
          
         translation = adj[word.substr(0, word.length() - 1) + "o"];
         word_type = 1;
        }
         else if(diminutive){
         if(nouns.count(word.substr(0, word.length() - 4)  + "o")){
             translation = "little " + nouns[word.substr(0, word.length() - 4) + "o"];

         }else if(nouns.count(word.substr(0, word.length() - 6) + "o") ){
             translation = "little " + nouns[word.substr(0, word.length() - 6) + "o"];
         }
           
            word_type = 0;
        }
       // if not found suffix match
        else if(suffixLookup(word).first.length() > 0){ 
         translation = suffixLookup(word).first;
       }else if(prefixLookup(word).first.length() > 0){
        // if suffix not found, look for prefix
        translation = prefixLookup(word).first;
        word_type = prefixLookup(word).second;
       }
   
    cout << translation + " " << word_type << " ";
  return {translation, word_type};
}

// when words need to switch order
// this is actually various manipulations (Take Off The Blindfold REFERENCE????)
//not only word order, but i'm not changing the name at this point
vector<pair<string, int>> reorder_helpers(vector<pair<string, int>> sentence_arr) {
    vector<pair<string, int>> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
      
    // ------------------------ ADJECTIVE ORDER  -----------------
    // a set is noun[0] and adjective[1], we switch order, so that casa[0] azul[1] -> blue[1] house[0]
      if (i > 0 && sentence_arr[i - 1].second == 0 && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 1]);
        } 

          // ------------------------ ARTICLE TWEAKS  --------------------
        // does the next translation start in a vowel? if so the article should be an. a apple -> an apple
        else if (i > 0 && sentence_arr[i - 1].second == 9 && isVowel(sentence_arr[i].first[0])) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i - 1].first + "n", 9});
            reordered_arr.push_back(sentence_arr[i]);  
        } 
       // ------------------------ OBLIQUE PRONOUNS  -----------------
       // a set is oblique pronoun[11] and verb[3], we switch order, so that te[11] amo[3] -> love[3] you[11]
        else if (i > 0 && sentence_arr[i - 1].second == 11 && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 1]);
        } 

          // ------------------------ SUPERLATIVE  --------------------
        // a set is word1 = 'o/a' and word2 = 'mais' and word3 = adj[1], we eliminate the 'mais', so that 'o mais forte[2]' -> 'the strongest'
        else if (i > 1 && sentence_arr[i - 2].second == 9  && sentence_arr[i - 1].first == "more" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i].first + "est", 20});  
        } 
        // ------------------------ COMPARATIVE  ----------------- TODO: exceptions such as beautiful[er], good[er], badd[er], might be better as bigrams
        // a set is word1 = 'mais' and word2 = adj[1], we eliminate the first 'mais' and append 'er' to word2, so that mais forte[2] -> stronger
        else if (i > 0 && sentence_arr[i - 1].first == "more" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i].first + "er", 20});  
        } 
      // ------------------------ INTRANSITIVE VERBS THAT TAKE 'DE'  ----------------- 
        // a set is word1 = verb[3] and word2 = 'de', we eliminate the preposition 'de', so that gosto[1] de  -> stronger
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].first == "of") {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
        } 
  // ------------------------ TRANSITIVE VERBS WITH A PERSONAL PRONOUN  ----------------- 
        // a set is word1 = verb[3] and word2 = pronoun[4], we use the second value of the pair? of the pronoun i guess idk i'm tired
        // like the value of the key 'ela' is a pair<string, string> that holds both the subject and object pronoun {'she', 'her'}
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].second == 4) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);
            reordered_arr.push_back(pair<string, int>{obj_pro[sentence_arr[i].first], 10});  
        } 


    // ------------------------ DOUBLE VERBS ----------------- TODO: figure out if verb1 verb[2]-ing is needed at some case
    // a set is verb[3] and verb[3], we add 'to' between them, so that amo[3] correr[3] -> love[3] *to* run[3]
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
            reordered_arr.push_back(pair<string, int>{"to", -1});  
            reordered_arr.push_back(sentence_arr[i]);
        } 
         else if (i > 1 && sentence_arr[i - 2].second == 3 && sentence_arr[i - 1].first == "of" && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 2]);  
            reordered_arr.push_back(pair<string, int>{"to", -1});  
            reordered_arr.push_back(sentence_arr[i]);
        } 
        
    // ------------------------ DOUBLE NOUNS ----------------- TODO: nuance? 
    // a set is noun[0] and "de" and noun[0], we invert them and remove the 'de/of' between them, so that "suco[0] de* laranja[0]" -> orange[0] juice[0]
           else if (i > 1 && sentence_arr[i - 2].second == 0  && sentence_arr[i - 1].first == "of" && sentence_arr[i].second == 0) {
            reordered_arr.pop_back();
            reordered_arr.pop_back();   
            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 2]);
        } 
         // ------------------------ NOT VS DON'T ----------------- TODO: i'm sure theres more cases where not is dont, and vice versa, ALSO THERES NO. 
         // FUCLKKKKKKKKKK THERES ALSO "DOESN'T" FOR THIRD PEROSN
    // a set is pronoun[4] + "no"  + verb[3]. 'no' becomes then 'don't' or doesn't so that não* gosto[3] -> don't like instead of 'no like'.
    
    //doesnt or dont
           else if (i > 1 && (sentence_arr[i - 2].second == 4 && sentence_arr[i - 1].first == "no")  && sentence_arr[i].second == 3) {
            cout << "it is happening again";
            string aux_verb;
            reordered_arr.pop_back();
            // does the pronoun before negation exist in the th_per_aux vector array? if so you push 'does' in there as result
                  if (std::find(th_per_aux.begin(), th_per_aux.end(), sentence_arr[i - 2].first) != th_per_aux.end()) {
                       aux_verb = "does";
                  }
                  // does the pronoun before negation exist in the reg_aux vector array? if so you push 'do' in there as result
                  else if (std::find(reg_aux.begin(), reg_aux.end(), sentence_arr[i - 2].first) != reg_aux.end()) {
                      aux_verb = "do";
                }
                   reordered_arr.push_back(pair<string, int>{aux_verb + "n't", 3});
                   reordered_arr.push_back(pair<string, int>{sentence_arr[i].first, 3});
        } 
        
        else {
            reordered_arr.push_back(sentence_arr[i]);
        }
    }

    return reordered_arr;
}

//ngram groups
std::string unigramLookup(vector<string> array_of_words, vector<int> ignore_flags){

  vector<pair<string, int>> sentence_arr;

  string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    pair<string, int> match = nounLookup(array_of_words[i]);
    switch (ignore_flags[i])
    {
    case 0:
    sentence_arr.push_back({match.first, match.second});
      break;
    case 1:
    sentence_arr.push_back({array_of_words[i], 0});
      break;
    default:
      break;
    }
  }
  sentence_arr = reorder_helpers(sentence_arr);
  for(size_t i = 0; i < sentence_arr.size(); ++i){
     sentence = sentence + " " + sentence_arr[i].first;
  }
  cout << "\n" << sentence << "\n";
  return sentence;
}

std::string bigramLookup(vector<string> array_of_words){
  vector<string> bi_array_of_words = array_of_words;
  // 0 is false lolz
  int should_ignore = 0;
  
  // if the sentence has 2 or more words, we can look up word bigrams
vector<string> mended_array_of_words;
vector<int> ignore_flags;

  size_t i = 1;
  while (i < bi_array_of_words.size()) {
      string bigram = bi_array_of_words[i-1] + "_" + bi_array_of_words[i];
      if (lookup(fixed_ngrams, bigram.c_str())) {
          mended_array_of_words.push_back(lookup(fixed_ngrams, bigram.c_str()));
          ignore_flags.push_back(1);  // mark this word as "already translated"
          i += 2;
      } else {
          mended_array_of_words.push_back(bi_array_of_words[i-1]);
          ignore_flags.push_back(0);  // normal word
          i += 1; 
      }
  }
  if (i == bi_array_of_words.size()) {
      mended_array_of_words.push_back(bi_array_of_words.back());
      ignore_flags.push_back(0);
  }

  return unigramLookup(mended_array_of_words, ignore_flags);
    }

std::string trigramLookup(vector<string> array_of_words){
    vector<string> tri_array_of_words = array_of_words;
  
    return bigramLookup(tri_array_of_words);
}

vector<string> traduzir_en(string sentence) {
    vector<string> arr;
    istringstream iss(sentence);
    string word;
    
    while (iss >> word) {
        arr.push_back(word);
    }

    for(size_t i = 0; i < arr.size(); ++i){
      // cout << arr[i] << ", ";
    }
   // cout << "\n";
    trigramLookup(arr);
    return arr;
}
//main body
  int main (){
   
    string original_sentence;

    std::cout << "O que deseja traduzir (pt-en)?\n";
    cout << "Digite 'sair' para encerrar.\n";
    while(true){
    getline(cin, original_sentence);
    traduzir_en(original_sentence + " ");
    
    if (original_sentence == "sair")
      break;
    }

    return 0;
  
}
