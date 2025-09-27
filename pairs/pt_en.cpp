#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>
#include <sstream> 
#include "../lang_it.h"

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
  {"por_causa", "because of"},
  {"o_pior", "the worst"},
  {"o_melhor", "the best"}
};
vector<string> modals = {"can", "must", "should", "could", "may", "will", "am", "is", "are"};

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
  {"olá", "hello"},
  {"mundo", "world"},
  {"dois", "two"},
  {"três", "three"},
  {"quatro", "four"},
  {"cinco", "five"},
  {"seis", "six"},
  {"seven", "seven"},
  {"oito", "eight"},
  {"nove", "nine"},
  {"dez", "ten"},
  {"melancia", "watermelon"},
  {"areia", "sand"},
  {"chuva", "rain"},
  {"acucar", "sugar"},
  {"cachorro", "dog"},
  {"agua", "water"},
  {"suco", "juice"},
  {"laranja", "orange"}, // how the hell will i handle that ?
  {"porta", "door"},
  {"janela", "window"},
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
  {"dinheiro", "money"},
  
  {"crianca", "kid"},
  {"criança", "kid"},
  {"amigo", "friend"},
  {"fome", "hunger"},
  {"frio", "cold"},
  {"dia", "day"},
  {"noite", "night"},
  {"olho", "eye"},
  {"coracao", "heart"},
  {"coisa", "thing"},
  {"estrela", "star"},
  {"livro", "book"},
  {"casa", "house"},
  {"principe", "prince"}, // TODO: irregular feminine noun shifts such as princesa, duquesa, garçonete, etc
  {"tradutor", "translator"},
  {"metade", "half"},
  {"meio", "middle"},
  {"bolo", "cake"}
};

map <string, string> art = {
  {"o", "the"},
  {"a", "the"},
  {"um", "a"},
  {"uma", "a"},
};

constexpr Entry pre[] = { 
  {"do", "of the"},
  {"da", "of the"},
  {"de", "of"},
  {"com", "with"}, // have to differentiate this from 'como' as in 'like' 
  {"sem", "without"},
  {"ou", "or"},
  {"em", "in"},
  {"no", "in the"},
  {"na", "in the"}
};

// nominative/personal pronouns
constexpr Entry pro[] = {
  {"eu", "i"},
  {"você", "you"},
  {"voce", "you"},
  {"tu", "you"},
  {"nós",  "we"},
  {"nos",  "we"},
  {"ele",  "he"},
  {"ela",  "she"},
  {"elas",  "they"},
  {"eles", "they"},
  {"esse", "this"},
  {"essa", "this"},
  {"este", "this"},
  {"esta", "this"},
  {"isso", "this"},
  {"isto", "this"}
};
constexpr Entry poss_pro[] = {
  {"seu", "your"},
  {"dela", "her"},
  {"dele",  "his"},
  {"nosso",  "our"},
  {"meu",  "my"},
  {"meus", "my"},
  {"minha",  "my"},
  {"minhas",  "my"}
};

//object pronoun match (in english)

map<string, string> obj_pro = {
  {"she", "her"},
  {"he", "him"},
  {"they", "them"},
  {"you", "you"},
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
  {"alto", "high"},
  {"correto", "correct"},
  {"sozinho", "alone"},
  {"facil", "easy"},
  {"dificil", "hard"},
  {"bem", "well"},
  {"humido", "humid"},
  {"melhor", "better"},
  {"pior", "worse"},
  {"estranho", "weird"},
  {"esquisito", "weird"},
  {"lento", "slow"},
  {"proprio", "own"}
  
};

//adverbs

constexpr Entry adv[] = {
  {"se", "if"},
  {"talvez", "maybe"},
  {"que", "what"},
  {"mas", "but"},
  {"quando", "when"},
  {"quem", "who"},
  {"também", "too"},
  {"porque", "because"},
  {"onde", "where"},
  {"quantos", "how many"},
  {"e", "and"},
  {"quanto", "how much"},
  {"nunca", "never"},
  {"sempre", "always"},
  {"aqui", "here"},
  {"ali", "there"},
  {"desde", "since"},
  {"ninguém", "nobody"},
  {"fora", "out"},
  {"pouco", "bit"},
  {"até", "until"},
  {"muito", "very"},
  {"assim", "like this"}
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
  {"molh", "wet", 1},
  {"pergunt", "ask", 1},
  {"pe", "ask", 1},
  {"precis", "need", 1},
  {"morr", "di", 0},
  {"sonh", "dream", 1},
  {"grit", "scream", 1},
  {"acredit","believ", 0},
  {"viv", "liv", 0},
  {"mor", "liv", 0},
  {"tent", "try", 1},
  {"compreend", "comprehend", 1},
  {"busc", "search", 1},
  {"brilh", "glow", 1},
  {"possu", "own", 1},
  {"aprend", "learn", 1},
  {"apag", "eras", 0},
  {"pint", "paint", 1},
  {"traduz", "translat", 0},
  {"mastig", "chew", 1},
  {"engol", "swallow", 1},
  {"respond", "answer", 1},
  {"desej", "wish", 1},
  {"trabalh", "work", 1},
  {"mov", "mov", 0},
  {"digit", "typ", 0},
  
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
  {"dev", {"should", 1}},
  {"pens", {"think", 1}},
  {"v", {"see", 1}},
  {"t", {"hav", 0}},
  {"funcion", {"work", 1}}
};

// 0 == pt 1 = ode 2 = ct 3 == ate 4 == ed 5 == icate 6 == ify 7 == itute 8 == er
// quick dirty verb guessing
map <string, int> patt_verbs = {
  {"ptar", 0}, // adaptar = adapt,
  {"odir", 1}, // explodir = explode 
  {"tar", 2}, // contatar = contact
  {"trair", 2}, // extrair = extract
  {"trar", 3}, // contatar = contact // what about encontrar.... (needs more specification)
  {"ificar", 6},
  {"ituir", 7},
  {"erir", 8},
  //surely theres a way to imolement the past tense endings dinamically, will i? who knows
  {"ptou", 4}, {"ptei", 4}, {"ptado", 4},
  {"odiu", 4}, {"odi", 4}, {"dido", 4},
  {"tou", 4}, {"tei", 4}, {"tado", 4},
  {"traiu", 4}, {"traí", 4},
  {"trou", 4}, {"trei", 4},
};


// common suffixes with traceable trnaslation pattern
constexpr Entry suff[] = {
  {"dade", "ty"},
  {"mente", "ly"},
  {"mento", "ment"},
  {"ável", "able"},
  {"ível", "ible"},
  {"ória", "ory"},
  {"ência", "ency"},
  {"cidade", "city"},
  {"açado", "aced"},
  {"ágico", "agic"},
  {"ágica", "agic"},
  {"ção", "tion"},
  {"culo", "cle"},
  {"cula", "cle"},
  {"cleta", "cle"},
  {"tério", "tery"},
  {"téria", "tery"},
  {"ário", "ary"},
  {"ária", "ary"},
  {"ral", "ral"},
  {"ador", "ator"},
  {"etro", "meter"},
  {"ência", "ency"},
  {"êncio", "ence"},
  {"fia", "phy"},
  {"eta", "et"},
  {"ndida", "ndid"},
  {"ndido", "ndid"},
  {"fico", "fic"},
  {"feito", "fect"},
  {"feita", "fect"},
  {"édia", "edy"},
  {"édio", "edy"},
  {"ura", "ure"}
};

string script_adequation(string word) {
    auto replace_all = [&](const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = word.find(from, pos)) != std::string::npos) {
            word.replace(pos, from.size(), to);
            pos += to.size();
        }
    };

    replace_all("\xC3\xA1", "a"); // á
    replace_all("\xC3\xA0", "a"); // à
    replace_all("\xC3\xA3", "a"); // ã
    replace_all("\xC3\xA2", "a"); // â
    replace_all("\xC3\xA9", "e"); // é
    replace_all("\xC3\xAA", "e"); // ê
    replace_all("\xC3\xAD", "i"); // í
    replace_all("\xC3\xB3", "o"); // ó
    replace_all("\xC3\xB4", "o"); // ô
    replace_all("\xC3\xB5", "o"); // õ
    replace_all("\xC3\xBA", "u"); // ú
    replace_all("\xC3\xA7", "c"); // ç
replace_all("\xC3\x81", "a"); // Á
replace_all("\xC3\x80", "a"); // À
replace_all("\xC3\x83", "a"); // Ã
replace_all("\xC3\x82", "a"); // Â
replace_all("\xC3\x89", "e"); // É
replace_all("\xC3\x8A", "e"); // Ê
replace_all("\xC3\x8D", "i"); // Í
replace_all("\xC3\x93", "o"); // Ó
replace_all("\xC3\x94", "o"); // Ô
replace_all("\xC3\x95", "o"); // Õ
replace_all("\xC3\x9A", "u"); // Ú
replace_all("\xC3\x87", "c"); // Ç
    return word;

}

bool isVowel(char x)
{
    if (x == 'a' || x == 'e' || x == 'i' || x == 'o'
        || x == 'u' || x == 'A' || x == 'E' || x == 'I'
        || x == 'O' || x == 'U')
    return true;
    else
     return false;
}

void to_lower(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
}

//normalization
//this will turn sets of letters that shift on translation and change them accordingly.
// stuff such as aceitar -> aceipt -> accept
string normalize(string word) {
    string normalized_ = word;

    if (word.length() > 3) {
        char thirdLast = word[word.size() - 3];
        string last2 = word.substr(word.size() - 2);
        if (isVowel(thirdLast) && last2 == "ed") {
            normalized_ = word.substr(0, word.size() - 3) + "ed";
        }

        if (word.substr(0, 3) == "esp") {
            normalized_ = normalized_.substr(1);  
        }

        if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 5) == "icaly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 5) + "ically";
        }
         if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 4) == "taly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 4) + "ctly";
        }
          if (normalized_.size() >= 5 && normalized_.substr(normalized_.size() - 3) == "yly") {
            normalized_ = normalized_.substr(0, normalized_.size() - 3) + "ily";
        }

       /* for (size_t i = 0; i + 1 < normalized_.size(); ) {
        if (normalized_[i] == 'l' && normalized_[i+1] == 'f' && normalized_[i+1] == 'a') {
            normalized_.replace(i, 2, "f");
            i++;
        } else {
            i++;
        }
    }*/
    }

    return normalized_;
}

pair<string, int> createNounFromVerb(string verb){
   string n = "";
   int word_type_;

  return pair<string, int>{n, word_type_};
}

pair<string, int> adjectification(string adj){
  
  string a = "";
  string v;
  int word_type_;

  if(adj.length() > 4){
      if(adj.substr(adj.length() - 3) == "ado" && lookupRegVerb(adj.substr(0, adj.length() - 3).c_str())){
        const Verb* verb = lookupRegVerb(adj.substr(0, adj.length() - 3).c_str()); // fech
        //clos + ed 
        a = string(verb->translation) + "ed";

        word_type_ = 1;
        }
    }else{
      a = "";
      word_type_ = -1;
    }

  return pair<string, int>{a, word_type_};
}

// dictionary lookup

// this is about to be nuts, maybe
// we'll do basic fragmentation, using the list of preffixes, and try to find a match
// i want this to be separate from prefixLookup() cause this will be >=3 and i need to lookup more than one table
//e.g inquebrável -> unbreakable. 
// we need to match the preffix "in" to "un", remove the last vowel and check if the root exists, (quebrá -> quebr -> break).
// and get the suffix and map it "ável" -> able
// putting this all together we get unbreakable.
// preffixes that are highly productive, [in -> un], [des -> de]
//yeah i'll need to make up rules like crazy here. theres no pattern at all
// you have un, in, ir and they just work by vibes i guess, unreal, incorrect, uncanny
// like a bunch of starting with r adjectives like real, will take ir, such as irrational, irresponsible 
// but what about exceptions such as unreal? unrequited?
// without
pair<string, int> morphemeLookup(string word){
  string translation_; 
  string p;
  string m;
  string s;
  int word_type_;
  
  if(word.length() > 3){
       // this will lookup the basic adjective negation (?), whats even the name of this linguistically, idk
       // but like, if you find the preffix (un) and the rest of the word is an adjective
       // you just put them together: incorreto -> [in] + [correct]
        if(word.substr(0, 2) == "in" && adj.count(word.substr(2, word.length()))){
         
             p = "in";
             m = adj[word.substr(2, word.length())];
             translation_ = p + m;
             word_type_ = 1;
          }
          else if (word.substr(0, 2) == "in") {
    string stem = word.substr(2);  


    for (auto &entry : suff) {
        const string &suffix = entry.w;

        if (stem.size() >= suffix.size() &&
            stem.compare(stem.size() - suffix.size(), suffix.size(), suffix) == 0) {
            
            string base = stem.substr(0, stem.size() - suffix.size());
            p = "un";

            string m;
            if (irr_verbs.count(base)) {
                m = irr_verbs[base].first;  
            } else {
                const Verb* v = lookupRegVerb(base.c_str());
                m = v ? v->root : base; 
            }
            s = m + entry.t;               
            translation_ = p + s;          
            word_type_ = 1;
            break;
        }
    }
}else if(word.substr(word.length() - 3) == "dor" && lookupRegVerb(word.substr(0, word.length() - 4).c_str())){
  
  const Verb* v = lookupRegVerb(word.substr(0, word.length() - 4).c_str());
  translation_ =  string(v->translation) + "er";
  word_type_ = 0;
}
else{
             translation_ = "";
             word_type_ = -1;
          }
  };
  

  return pair<string, int>{translation_, word_type_};
};

//look for preffix matches 
pair<string, int> prefixLookup(string word){

  
  string translation = word; 
  int word_type;

  vector<string> infinitive = {"ar", "er", "ir", "dir", "r", "ir"};
  vector<string> present_non_s = {"o", "to", "go", "ro", "am", "em", "amos", "emos", "mo", "lo", "ço", "nho", "so", "ejo", "enho"};
  vector<string> present_s = {"a","as", "ta", "tas", "re", "ga", "ui", "uis", "ê", "ês", "em"};
  vector<string> general_past = {"ei", "ou", "eu", "ti", "aram", "ri", "i", "iu", "imos", "inha"};
  vector<string> present_continuous = {"ndo", "ndo", "ando"};
  vector<string> completed_past = {"ava", "ávamos", "íamos", "nhamos","ia"};
  vector<string> subjunctive = {"sse", "ssemos"};
  vector<string> conditional = {"aria", "ariamos", "eria"};
  vector<string> imperative = {"e", "a", "eja", "enha", "á"};

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
        if (v->type == 0) {
          buffer[i++] = 'e';
        }
        buffer[i] = '\0';
    }
    else if(verb_info == 6){
       const char* would = "would ";
        size_t i = 0;
        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
        const char* base = v->translation;
        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
        if (v->type == 0) {
            buffer[i++] = 'e';
        }
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

} else if(irr_verbs.find(root) != irr_verbs.end()){
        
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
             case 7:
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
                      ( //are they followed by either  || "i" || "ee"
                        irr_verbs[root].first.substr(2, 1) == "i" || 
                        irr_verbs[root].first.substr(2, 2) == "ee") &&
                      irr_verbs[root].first.back() != 'd' &&  // do they NOT end in 'd' || 'g' || 'p' ||
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
            return pair<string, int>{translation_, word_type_};
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
                case 5: ending = "icate";   break;
                case 6: ending = "ify";   break;
                case 7: ending = "itute";   break;
                 case 8: ending = "er";   break;
              }
            
            translation_ = normalize(stem + ending);
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
    result_set = find_verb(imperative, word, 7);


if (result_set.first.empty()) 
    result_set = pair<string, int>{irr_verbs[word].first, 3};

  return result_set;
}
pair<string, int> suffixLookup(const string& word) {
    string translation;
    int word_type = -1;

    if (word.size() > 4) {
        if (word.substr(0, 2) == "in" && lookup(suff, word.substr(word.size() - 4).c_str())) {
            return {"", -1};
        }
    }

    // this is some lexeme break up shit
    // TODO: expand to nouns and verbs as well. like abertamente, currently becomes (abertaly -> abertcly).
    // you should be able to find the adj stem abert from abrir.
    // not very hard since theres patterns for verb->adjective conversion, such as abrir -> aberto, cobrir -> coberto
    // this way you can store just the verb stems and no need of storing a bunch of adjectives
    for (int len = 6; len >= 2; --len) {
        if (word.length() >= len) {
// like if you find ly, in felizmente -> tristely. you break up [triste] [ly]. 
          string ending = word.substr(word.length() - len);
            const char* mapped = lookup(suff, ending.c_str());

            if (mapped) {
            // look up [triste] in the adj table, and if theres a match [sad]
                string stem = word.substr(0, word.length() - len);
               
               if (adj.count(stem)) {
            translation = adj[stem] + mapped;
        } else if (!stem.empty() && adj.count(stem.substr(0, stem.length() -1) + "o")) { 
            translation = adj[stem.substr(0, stem.length() -1) + "o"] + mapped;
        } else {
            translation = stem + mapped;
        }

                word_type = 2;
                return {normalize(script_adequation(translation)), word_type};
            }
        }
    }

    return {"", -1}; // fallback
}

bool isDiminutive(const std::string& s, const char* suffix) {
    size_t n = 0;
    while (suffix[n] != '\0') n++; // strlen without <cstring>
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; i++) {
        if (s[s.size() - n + i] != suffix[i]) return false;
    }
    return true;
}
pair<string, int> nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type;


  
  
  // rules
   bool plural = ((nouns.count(word.substr(0, word.length() - 1)) || nouns.count(word.substr(0, word.length() - 2) + "o")) && word.substr(word.length() - 1) == "s"); // this is plural nouns only
   bool gender_shift = nouns.count(word.substr(0, word.length() - 1)  + "o"); // this is gender shift for nouns only

bool diminutive = false;
    if (isDiminutive(word, "inho")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 4) + "o"));
    }
    else if (isDiminutive(word, "inha")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 4) + "a"));
    }
    else if (isDiminutive(word, "inhos")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 5) + "o"));
    }
    else if (isDiminutive(word, "inhas")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 5) + "a"));
    }
    else if (isDiminutive(word, "zinho")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 5) + "o"));
    }
    else if (isDiminutive(word, "zinha")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 5) + "a"));
    }
    else if (isDiminutive(word, "zinhos")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 6) + "o"));
    }
    else if (isDiminutive(word, "zinhas")) {
        diminutive = nouns.count(script_adequation(word.substr(0, word.size() - 6) + "a"));
    }

   bool adj_plural = ((adj.count(word.substr(0, word.length() - 1)) || adj.count(word.substr(0, word.length() - 2) + "o")) && word.substr(word.length() - 1) == "s"); // this is plural adjectives only
   bool adj_gender_shift = adj.count(word.substr(0, word.length() - 1)  + "o"); // this is gender shift for adjectives only
   bool article_plural = art.count(word.substr(0, word.length() - 1));
  

  // for each individual word loop, you look in the noun dictionary
  //first with accentuation, 
  if(nouns.count(word)){
   translation = nouns[word];
   word_type = 0;
   }
   //then without accentuation (helpful in plural)
   else if(nouns.count(script_adequation(word))){
       translation = nouns[script_adequation(word)];
       word_type = 0; 
   }
   else if(adj.count(script_adequation(word))){
      translation = adj[script_adequation(word)];
      word_type = 1;
    }
    else if(lookup(pro, script_adequation(word).c_str())){
      translation = lookup(pro, script_adequation(word).c_str());
      word_type = 4;
    } else if(lookup(poss_pro, word.c_str())){
      translation = lookup(poss_pro, word.c_str());
      word_type = 4;
    }
  
    else if(obl_pro.count(word)){
      translation = obl_pro[word];
      word_type = 11;
    }

     else if(lookup(pre, word.c_str())){
      translation = lookup(pre, word.c_str());
      word_type = 8;
    }
     else if(art.count(word)){
      translation = art[word];
      word_type = 9;
    }
     else if(article_plural){
      translation = art[word.substr(0, word.length() - 1)];
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
    string singular_en = nouns[script_adequation(singular_pt) + (word.substr(word.size()-2)=="as" || word.substr(word.size()-2)=="os" ? "o" : "")];
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
         if(nouns.count(script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "o")){
             translation = "little " + nouns[script_adequation(word).substr(0, script_adequation(word).length() - 4) + "o"];

         }else if(nouns.count(word.substr(0, word.length() - 6) + "o") ){
             translation = "little " + nouns[word.substr(0, word.length() - 6) + "o"];
         }
           else  if(nouns.count(script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "a")){
             translation = "little " + nouns[script_adequation(word).substr(0, script_adequation(word).length() - 4) + "a"];

         }else  if(nouns.count(script_adequation(word).substr(0, script_adequation(word).length() - 5))){
             translation = "little " + nouns[script_adequation(word).substr(0, script_adequation(word).length() - 5)];

         }
            word_type = 0;
        }
        else if(morphemeLookup(word).first.length() > 0){
        // if suffix not found, look for morpheme breakdown
        translation = morphemeLookup(word).first;
        word_type = morphemeLookup(word).second;
      }

       // if not found suffix match
        else if(suffixLookup(word).first.length() > 0){ 
         translation = suffixLookup(word).first;
          word_type = suffixLookup(word).second;
       }
       
       else if(prefixLookup(word).first.length() > 0){
        // if suffix not found, look for prefix
        translation = prefixLookup(word).first;
        word_type = prefixLookup(word).second;
       }
       else if(adjectification(word).first.length() > 0){
        // if preffix not found, look for prefix
        translation = adjectification(word).first;
        word_type = adjectification(word).second;
       }
       
       else{
           return {word, -2};
      }
  return {translation, word_type};
}

// when words need to switch order
// this is actually various manipulations (Take Off The Blindfold REFERENCE????)
//not only word order, but i'm not changing the name at this point
vector<pair<string, int>> reorder_helpers(vector<pair<string, int>> sentence_arr) {
    vector<pair<string, int>> reordered_arr;

    // interrogative
    if(sentence_arr.size() > 0){
      string last = sentence_arr[sentence_arr.size() - 1].first;
         //　QUESTION　QUESTION　僕は　QUESTION　QUESTION　いったい　QUESTION　QUESTION　君の何を知っていたの????????????
         
         // 
         if(last == "?"){
          // look for a pronoun
          string pronoun_ = "";
          for (auto &p : sentence_arr) {
              if (p.second == 4) {
                  pronoun_ = p.first; 
                  break;                
              }
          }
          if (!pronoun_.empty()) {
            // is this pronoun in the third person vector?
          string aux = (std::find(th_per_aux.begin(), th_per_aux.end(), pronoun_) != th_per_aux.end()) ? "does" : "do";
          
          cout << aux;
          }
        }
    }

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
      
    // ------------------------ PRONOUN ASSIGNING  -----------------
      // english verbs do not conjugate person aside from third vs non-third (and even thats an understatement
      // cause 'they' (3rd plural) falls with 1st. e.g she loves, he loves, you love, i love, we love, they love 
      // point being a portuguese pronoun can infer more info on the person: eu comO, você comE, eles comEM, nos comEMOS.

      // a single verb can define person: vejo -> I see
     if (sentence_arr.size() == 1 && sentence_arr[0].second == 3) {
          reordered_arr.clear(); 
          reordered_arr.push_back(sentence_arr[0]);
          return reordered_arr;
      }
        else if (i > 0 && sentence_arr[i - 1].second == 0 && sentence_arr[i].second == 1) {
                      reordered_arr.pop_back(); 
                      reordered_arr.push_back(sentence_arr[i]);  
                      reordered_arr.push_back(sentence_arr[i - 1]);
                  } 


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

        // ------------------------ CONTINOUS TO BE (IS, ARE, AM)  ----------------- 
        // a set is word1 = pronoun[4] and word2 = "is", use an if to check the first pronoun and change it accordingly 
        // she is, i am, we are
        else if (i > 0 && sentence_arr[i - 1].second == 4 && sentence_arr[i].first == "is") {
            string corr_pro = "is";
            reordered_arr.pop_back(); 
            if(sentence_arr[i - 1].first == "she" || sentence_arr[i - 1].first == "he"){
                    corr_pro = "is";
            } else if(sentence_arr[i - 1].first == "we" || sentence_arr[i - 1].first == "they" || sentence_arr[i - 1].first == "you"){
                    corr_pro = "are";
            }else if(sentence_arr[i - 1].first == "i"){
                    corr_pro = "am";
            };
            
            reordered_arr.push_back(sentence_arr[i - 1]);   
            reordered_arr.push_back(pair<string, int>{corr_pro, 4});
        } 


    // ------------------------ DOUBLE VERBS ----------------- TODO: figure out if verb1 verb[2]-ing is needed at some case
    // a set is verb[3] and verb[3], we add 'to' between them, so that amo[3] correr[3] -> love[3] *to* run[3]
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
            
              if (std::find(modals.begin(), modals.end(), sentence_arr[i - 1].first) == modals.end()) {
                      reordered_arr.push_back(pair<string, int>{"to", -1});  
              }
            reordered_arr.push_back(sentence_arr[i]);
        } 
         // same thing but when the verbs are connected by 'que' (e.g: tenho QUE ver)
        else if (i > 1 && sentence_arr[i - 2].second == 3 && sentence_arr[i - 1].first == "what" 
      && sentence_arr[i].second == 3) 
{
    reordered_arr.pop_back(); 
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
   // -------------------- KIND OF MEIO [ADJ] -----------------------------------------
         else if (i > 0 && sentence_arr[i - 1].first == "meio" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{"kind of", -1});  
            reordered_arr.push_back(sentence_arr[i]);
        } 
         // ------------------------ NOT VS DON'T ----------------- TODO: i'm sure theres more cases where not is dont, and vice versa, ALSO THERES NO. 
         // FUCLKKKKKKKKKK THERES ALSO "DOESN'T" FOR THIRD PEROSN
    // a set is pronoun[4] + "no"  + verb[3]. 'no' becomes then 'don't' or doesn't so that não* gosto[3] -> don't like instead of 'no like'.
    
   else if (i > 1 && sentence_arr[i - 2].second == 4 && sentence_arr[i - 1].first == "não" && sentence_arr[i].second == 3) {
    string subj = sentence_arr[i - 2].first;
    string verb = sentence_arr[i].first;

    while (!reordered_arr.empty() && 
          (reordered_arr.back().first == subj || reordered_arr.back().first == "não")) {
        reordered_arr.pop_back();
    }

    string aux;
    if (std::find(modals.begin(), modals.end(), verb) != modals.end()) {
        aux = verb + " not";
        reordered_arr.push_back({subj, 4});
        reordered_arr.push_back({aux, 3});
    } else {
        aux = (std::find(th_per_aux.begin(), th_per_aux.end(), subj) != th_per_aux.end()) ? "doesn't" : "don't";
        reordered_arr.push_back({subj, 4});
        reordered_arr.push_back({aux, 3});
        reordered_arr.push_back({verb, 3});
    }

    i++; 
} else {
    reordered_arr.push_back(sentence_arr[i]);
}

    }

    return reordered_arr;
}

std::vector<std::string> tokenize(const std::string &text) {
    std::vector<std::string> tokens;
    std::string current;
    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = text[i];

        if ((c & 0x80) == 0) {
            // ASCII
            if (std::isalnum(c)) {
                current += c;
            } else {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                if (!std::isspace(c))
                    tokens.push_back(std::string(1, c));
            }
            ++i;
        } else {
            // UTF-8 multibyte
            size_t len = 0;
            if ((c & 0xE0) == 0xC0) len = 2;
            else if ((c & 0xF0) == 0xE0) len = 3;
            else if ((c & 0xF8) == 0xF0) len = 4;
            else len = 1; // fallback

            std::string utf8char = text.substr(i, len);
            current += utf8char;
            i += len;
        }
    }

    if (!current.empty())
        tokens.push_back(current);

    return tokens;
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
 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const std::string& token = sentence_arr[i].first;

    char firstChar = token.empty() ? '\0' : token[0];
    bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                          firstChar == '.' || firstChar == ',');

    if (!sentence.empty() && !isPunctuation) {
        sentence += " ";
    }

    sentence += token;
}
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
std::string traduzir_en(std::string sentence) {
    to_lower(&sentence[0]); 
    std::vector<std::string> arr = tokenize(sentence);  
    std::string translated = trigramLookup(arr);
    
    return script_adequation(translated); 
}