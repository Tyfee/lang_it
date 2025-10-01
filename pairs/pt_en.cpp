#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
  {"o_melhor", "the best"},
  {"até_mesmo", "even"},
  {"por_enquanto", "for now"},
  {"hoje_em_dia", "nowadays"},
  {"de_vez_em_quando", "sometimes"}
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
constexpr Entry nouns[] = {
  {"olá", "hello"},
  {"inglês", "english"},
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
  {"chuva", "rain"}, // this is a verbifiable
  {"acucar", "sugar"},
  {"cachorro", "dog"},
  {"agua", "water"}, // this is a verbifiable ig?
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
  {"bolo", "cake"},
  {"vez", "time"},
  {"algo", "something"},
  {"biblioteca", "library"},
  {"detalhe", "detail"},
  {"estação", "season"},
  {"mesa", "table"},
  {"cadeira", "chair"},
  {"tudo", "all"},
  {"perdao", "forgiveness"}
};

constexpr Entry art[] = {
  {"o", "the"},
  {"a", "the"},
  {"um", "a"},
  {"uma", "a"}
};

constexpr Entry pre[] = { 
  {"do", "of the"},
  {"da", "of the"},
  {"de", "of"},
  {"com", "with"}, // have to differentiate this from 'como' as in 'like', how tho......... ominous
  {"sem", "without"},
  {"ou", "or"},
  {"em", "in"},
  {"no", "in the"},
  {"na", "in the"},
  {"nos", "in the"},
  {"nas", "in the"}
};

// nominative/personal pronouns
constexpr Entry pro[] = {
  {"eu", "i"},
  {"você", "you"},
  {"voce", "you"},
  {"tu", "you"},
  {"nós",  "we"},
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

constexpr Entry obj_pro[] = {
  {"she", "her"},
  {"he", "him"},
  {"they", "them"},
  {"you", "you"},
};



vector<string> th_per_aux = {"she", "he", "it"};
vector<string> reg_aux = {"i", "you", "we", "they"};

// oblique pronouns
constexpr Entry obl_pro[] = {
  {"te", "you"},
  {"me", "me"},
};

// adjectives
constexpr Entry adj[] = {
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
  {"proprio", "own"},
  {"sério", "serious"},
  {"doente", "sick"},
  {"certo", "certain"},
  {"outro", "other"},
  {"sobre", "about"},
  {"gratis", "free"},
  {"livre", "free"},
  {"ultimo", "last"}
};

//adverbs

constexpr Entry adv[] = {
  {"se", "if"},
  {"talvez", "maybe"},
  {"que", "that"},
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
  {"assim", "like this"},
  {"já", "already"},
  {"dentro", "inside"},
  {"fora", "outside"},
  {"hoje", "today"},
  {"ontem", "yesterday"},
  {"amanhã", "tomorrow"},
  {"agora", "now"},
  {"antes", "before"},
  {"depois", "after"},
  {"para", "to"},
  {"pra", "to"},
  {"por", "for"},
  {"ainda", "still"},
  {"somente", "only"},
  {"só", "only"},
  {"apenas", "just"}
};

struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
    bool intransitive;
};
struct VerbEnding {
    const char* ending;
    int code;
};

// verb prefixes where 0 = regular, 1 = irregular conjugation
// is it intransitive?
constexpr Verb reg_verbs[]  = {
  {"am", "lov", 0, false},
  {"gost", "lik", 0, false},
  {"qu", "want", 1, false},
  {"corr", "run", 1, true},
  {"abr", "open", 1, false},
  {"fech", "clos", 0, true},
  {"molh", "wet", 1, false},
  {"pergunt", "ask", 1, true},
  {"pe", "ask", 1, false},
  {"precis", "need", 1, false},
  {"morr", "di", 0, true},
  {"sonh", "dream", 1, true},
  {"grit", "scream", 1, true},
  {"acredit","believ", 0, true},
  {"viv", "liv", 0, true},
  {"mor", "liv", 0, false},
  {"tent", "try", 1, true},
  {"compreend", "comprehend", 1, false},
  {"busc", "search", 1, false},
  {"brilh", "glow", 1, true}, // a lot of these verbs can be inferred from their nouns: brilho -> brilhar, glow -> glow
  {"possu", "own", 1, false}, // remind to implement that, me;
  {"aprend", "learn", 1, false},
  {"apag", "eras", 0, false},
  {"pint", "paint", 1, true},
  {"traduz", "translat", 0, true},
  {"mastig", "chew", 1, false},
  {"engol", "swallow", 1, false},
  {"respond", "answer", 1, false},
  {"desej", "wish", 1, true},
  {"trabalh", "work", 1, true},
  {"mov", "mov", 0, false},
  {"digit", "typ", 0, false},
  {"olh", "look", 1, true}, //olho -> olhar even 
  {"sint", "feel", 1, false},
  {"mat", "kill", 1, false},
  {"prefer", "prefer", 1, false},
  {"prefir", "prefer", 1, false}
  
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


constexpr Verb irr_verbs[] = {
  {"fal", "speak", 1, false},
  {"beb", "drink", 1, false},
  {"sangr", "bleed", 1, true},
  {"procri", "breed", 1, true},
  {"aliment", "feed", 1, false},
  {"nad", "swim", 1, true},
  {"quebr", "break", 1, false},
  {"escrev", "writ", 1, true},
  {"dirig", "driv", 0, true},
  {"dirij", "driv", 0, true},
  {"est","%", 1, true}, // % is a flag for the 'to be' verb, i don't want to figure out a clever way to do that right now, so i'll simply mark it with a flag
                    // and deal with conjugation based on what's goin on around it (i - 1) and (i + 1), since it could match:
                    // está (3rd person singular => is) estamos (1st person plural => are) estão => (3rd person plural => are) or estou (1st person singular => am)

  {"cant", "sing", 1, true},
  {"ganh", "win", 1, true},
  {"é", "is", 1, true},
  {"são", "are", 1, true},
  {"sou", "am", 1, true},
  {"volt", "go back", 1, true},
  {"te", "have", 1, false},
  {"com", "eat", 1, false},
  {"lut", "fight", 1, true},
  {"pod", "can", 1, false},
  {"pos", "can", 1, false},
  {"dev", "should", 1, true},
  {"pens", "think", 1, true},
  {"v", "see", 1, false},
  {"t", "hav", 0, false},
  {"funcion", "work", 1, true},
  {"desenh", "draw", 1, true}
};

const Verb* lookupIrrVerb(const char* root) {
    for (const auto& v : irr_verbs) {
        const char* p = v.root;
        const char* q = root;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return &v; 
    }
    return nullptr;
}

// 0 == pt 1 = ode 2 = ct 3 == ate 4 == ed 5 == icate 6 == ify 7 == itute 8 == er 9 = it
// quick dirty verb guessing
constexpr VerbEnding patt_verbs[] = {
  {"ptar", 0}, // adaptar = adapt,
  {"odir", 1}, // explodir = explode 
  {"tar", 2}, // contatar = contact
  {"trair", 2}, // extrair = extract
  {"trar", 3}, // contatar = contact // what about encontrar.... (needs more specification)
  {"ificar", 6},
  {"ituir", 7},
  {"erir", 8},
  {"itar", 9},

  //surely theres a way to imolement the past tense endings dinamically, will i? who knows
  {"ptou", 4}, {"ptei", 4}, {"ptado", 4},
  {"odiu", 4}, {"odi", 4}, {"dido", 4},
  {"tou", 4}, {"tei", 4}, {"tado", 4},
  {"traiu", 4}, {"traí", 4},
  {"trou", 4}, {"trei", 4},

  //imperative ig, just adding 10 to the original
  {"pte", 10},
  {"oda", 11}, 
  {"te", 12},
  {"traia", 12},
  {"tre", 13}, 
  {"ifique", 14},
  {"itua", 17},
  {"ira", 18},
  {"ite", 19}
};

const VerbEnding* lookupEnding(const char* word) {
    for (const auto& ve : patt_verbs) {
        size_t word_len = 0;
        while (word[word_len] != '\0') ++word_len;

        size_t end_len = 0;
        while (ve.ending[end_len] != '\0') ++end_len;

        if (word_len >= end_len) {
            const char* p = ve.ending;
            const char* q = word + word_len - end_len;
            while (*p && *q && *p == *q) { ++p; ++q; }
            if (*p == '\0') return &ve;
        }
    }
    return nullptr;
}
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
  {"ura", "ure"},
  {"ês", "ese"}
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

//is the word visibly not imported from a different language? 
// had to do this since stuff like 'website' is conflicting with verb ending match 'ite',
// but if we infer wether or not the word is an import, we can stop that
bool isPortuguese(const string& word){

  bool isIt = true;

  for(int i = 0; i < word.length(); ++i){
    char letter = word[i];
    // out of the way we know that there arent native portuguese words with w,y or k. so........  
        if(letter == 'w' || letter == 'y' || letter == 'k'){
             return false;
      }
      // there must be other syllables that could fit here to extract this info to but i'll do that another day :p

    }
    return isIt;

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
        if (normalized_[i] == 'l' && normalized_[i+1] == 'f' && normalized_[i+2] == 'a') {
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
        if(word.substr(0, 2) == "in" && lookup(adj, (word.substr(2, word.length()).c_str()))){
         
             p = "in";
             m = lookup(adj, (word.substr(2, word.length()).c_str()));
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
            if (lookupIrrVerb(base.c_str())) {
                m = lookupIrrVerb(base.c_str())->translation;  
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
    vector<string> present_non_s = {"o", "to", "go", "ro", "am", "em", "amos", "emos", "mo", "lo", "ço", "nho", "so", "ejo", "enho", "ero"};
    vector<string> present_s = {"a","as", "ta", "tas", "re", "ga", "ui", "uis", "ê", "ês", "em"};
    vector<string> general_past = {"ei", "ou", "eu", "ti", "aram", "ri", "i", "iu", "imos", "inha", "is"};
    vector<string> present_continuous = {"ndo", "ndo", "ando"};
    vector<string> completed_past = {"ava", "ávamos", "íamos", "nhamos","ia"};
    vector<string> subjunctive = {"sse", "ssemos"};
    vector<string> conditional = {"aria", "ariamos", "eria"};
    vector<string> imperative = {"e", "a", "eja", "enha", "á"};

    // this will try to find a verb ending that can be translated to past tense or infinitive or continuous or whatever
    // it's a lambda that returns a pair with the match lemma + the vowel/conjugation.
    auto find_verb = [](vector<string> format, string word, int verb_info) -> pair<string,int> {
        for(size_t i = 0; i < format.size(); ++i){
            string translation_;
            int word_type_;
            int verb_type;
            bool aux = false;
            bool intransitiveness;
            string ending;

            size_t match = word.rfind(format[i]);
            if (match != string::npos && match + format[i].length() == word.length()) {
                string root = word.substr(0, match);
                auto v_irr = lookupIrrVerb(root.c_str());
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
                        if (v->type == 0) buffer[i++] = 'e';
                        buffer[i] = '\0';
                    }
                   else if (verb_info == 5) {
                          size_t i = 0;
                          const char* base = v->translation;
                          while (*base && i + 1 < sizeof(buffer)) {
                              buffer[i++] = *base++;
                          }
                          const char* ending = "ing";
                         
                           while (*ending && i + 3 < sizeof(buffer)) buffer[i++] = *ending++;

                          buffer[i] = '\0';
                      }
                    else if(verb_info == 6){
                        const char* would = "would ";
                        size_t i = 0;
                        while (*would && i + 1 < sizeof(buffer)) buffer[i++] = *would++;
                        const char* base = v->translation;
                        while (*base && i + 1 < sizeof(buffer)) buffer[i++] = *base++;
                        if (v->type == 0) buffer[i++] = 'e';
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

                    intransitiveness = v->intransitive;
                    if(intransitiveness == true){
                        word_type_ = 3;
                    }else{
                       word_type_ = 36;
                    }
                    verb_type = v->type;
                    return pair<string, int>{string(buffer), word_type_};

                } else if(v_irr){

                    if(root == "est"){
                        // estou == am estamos == are está == is estão == are
                        if (word.substr(3, word.length()) == "ou"){
                            translation_ = "am";
                        } else if (word.substr(3, word.length()) == "ava"){
                            translation_ = "was";
                        } else if(word.substr(3, word.length()) == "amos" || word.substr(3, word.length()) == "ão"){
                            translation_ = "are";
                        } else if(word.substr(3, word.length()) == "á"){
                            translation_ = "is";
                        }

                        word_type_ = 28; // 28 == to be cause 2 == TO 8 == B
                        return pair<string, int>{translation_, word_type_};
                    }

                    switch (verb_info)
                    {
                        case 0: ending = (v_irr->type == 0) ? "e" : ""; break;
                        case 1: ending = "ed"; break;
                        case 2: ending = (v_irr->type == 0) ? "e" : ""; break;
                        case 3: ending = (v_irr->type == 0) ? "es" : "s"; break;
                        case 4: ending = "e"; break;
                        case 5: ending = "ing"; break;
                        case 6: case 7: ending = (v_irr->type== 0) ? "e" : ""; break;
                        default: break;
                    }

                    if(verb_info == 4){
                        translation_ = "used to " + string(v_irr->translation) + ending;
                    } else if(verb_info == 6){
                        if (root == "pod") translation_ = "could";
                        else if(root == "dev") translation_ = "should";
                        else translation_ = "would " + string(v_irr->translation) + ending;
                        aux = true;
                    } else {
                        translation_ = string(v_irr->translation) + ending;
                    }

                    intransitiveness = v_irr->intransitive;
                    if(intransitiveness == true){
                        word_type_ = !aux ? 3 : 33;
                    }else{
                       word_type_ = 36;
                    }
                    verb_type = v_irr->type;
                    return pair<string, int>{translation_, word_type_};
                }
            }
        }
        return pair<string,int>{"", -1};
    };

    pair<string,int> result;

    result = find_verb(infinitive, word, 0);
    if(result.second != -1) return result;

    result = find_verb(present_non_s, word, 0);
    if(result.second != -1) return result;

    result = find_verb(present_s, word, 3);
    if(result.second != -1) return result;

    result = find_verb(general_past, word, 1);
    if(result.second != -1) return result;

    result = find_verb(present_continuous, word, 5);
    if(result.second != -1) return result;

    result = find_verb(completed_past, word, 4);
    if(result.second != -1) return result;

    result = find_verb(subjunctive, word, 2);
    if(result.second != -1) return result;

    result = find_verb(conditional, word, 6);
    if(result.second != -1) return result;

    result = find_verb(imperative, word, 7);
    if(result.second != -1) return result;

      const VerbEnding* ve = lookupEnding(word.c_str());
      if (ve) { 
          std::string stem; 
          std::string ending; 
          size_t ending_len = std::string(ve->ending).size();
          if (ve->code == 4) { 
              stem = word.substr(0, word.size() - (ending_len - 2)); 
          } else { 
              stem = word.substr(0, word.size() - ending_len); 
          } 
          switch(ve->code) { 
              case 0: case 10: ending="pt"; break; 
              case 1: case 11: ending="ode"; break; 
              case 2: case 12: ending="ct"; break; 
              case 3: case 13: ending="trate"; break; 
              case 4: case 14: ending="ed"; break; 
              case 5: case 15: ending="icate"; break; 
              case 6: case 16: ending="ify"; break; 
              case 7: case 17: ending="itute"; break; 
              case 8: case 18: ending="er"; break; 
              case 9: case 19: ending="it"; break; 
          } 
          if(stem.length() > 2) { 
              return {normalize(stem + ending), 3}; 
          } else { 
              return {normalize(word), 3}; 
          } 
      }
   

    return {"", -1};
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
               
               if (lookup(adj,stem.c_str())) {
            translation = string(lookup(adj, stem.c_str())) + mapped;
        } else if (!stem.empty() && lookup(adj,(stem.substr(0, stem.length() -1) + "o").c_str())) { 
            translation = string(lookup(adj, (stem.substr(0, stem.length() -1) + "o").c_str())) + mapped;
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
   bool plural = ((lookup(nouns, (word.substr(0, word.length() - 1)).c_str()) || lookup(nouns, (word.substr(0, word.length() - 2) + "o").c_str())) && word.substr(word.length() - 1) == "s"); // this is plural nouns only
   bool gender_shift = lookup(nouns, (word.substr(0, word.length() - 1)  + "o").c_str()); // this is gender shift for nouns only

bool diminutive = false;
    if (isDiminutive(word, "inho")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 4) + "o").c_str()));
    }
    else if (isDiminutive(word, "inha")) {
      diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 4) + "a").c_str()));
    }
    else if (isDiminutive(word, "inhos")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "o").c_str()));
    }
    else if (isDiminutive(word, "inhas")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "a").c_str()));
    }
    else if (isDiminutive(word, "zinho")) {
        diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "o").c_str()));
    }
    else if (isDiminutive(word, "zinha")) {
         diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 5) + "a").c_str()));
    }
    else if (isDiminutive(word, "zinhos")) {
      diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 6) + "o").c_str()));
    }
    else if (isDiminutive(word, "zinhas")) {
         diminutive = lookup(nouns, (script_adequation(word.substr(0, word.size() - 6) + "a").c_str()));
    }

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
   else if(lookup(nouns, script_adequation(word).c_str())){
       translation = lookup(nouns, script_adequation(word).c_str());
       word_type = 0; 
   }
   else if(lookup(adj, (script_adequation(word)).c_str())){
    
      translation = lookup(adj, script_adequation(word).c_str());
      word_type = 1;

    }
    else if(lookup(pro, word.c_str())){
      translation = lookup(pro, word.c_str());
      word_type = 4;
    } else if(lookup(poss_pro, word.c_str())){
      translation = lookup(poss_pro, word.c_str());
      word_type = 4;
    }
  
    else if(lookup(obl_pro, word.c_str())){
      translation = lookup(obl_pro, word.c_str());
      word_type = 11;
    }

     else if(lookup(pre, word.c_str())){
      translation = lookup(pre, word.c_str());
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
if (word.size() > 2 && word.substr(word.size() - 2) == "os") {
    singular_pt = word.substr(0, word.size() - 2) + "o"; // gatos -> gato, cachorros -> cachorro
} else if (word.size() > 2 && word.substr(word.size() - 2) == "as") {
    singular_pt = word.substr(0, word.size() - 2) + "a"; // casas -> casa, cachorras -> cachorra
} else if (word.size() > 1 && word.substr(word.size() - 1) == "s") {
    singular_pt = word.substr(0, word.size() - 1);       // flores -> flore (fallback)
}else {
    singular_pt = ""; // não é plural conhecido
}

if (lookup(nouns, singular_pt.c_str())) {
    string singular_en = lookup(nouns, script_adequation(singular_pt).c_str());
    if (!singular_en.empty()) {
        if (singular_en.size() >= 2 && singular_en.substr(singular_en.size()-2) == "fe") {
            translation = singular_en.substr(0, singular_en.size()-2) + "ves";
        } else if (singular_en.back() == 'f') {
            translation = singular_en.substr(0, singular_en.size()-1) + "ves";
        } else {
            translation = singular_en + "s";
        }
        word_type = 0;
    }
}

}
    // same as above for adjectives. e.g bonito[s] -> bonito, except we dont plug in 's' cause english has no adj. plurals ;p
      else if(adj_plural){

        if(lookup(adj, (word.substr(0, word.length() - 1).c_str()))){
         translation = lookup(adj, (word.substr(0, word.length() - 1).c_str()));

        }else if(lookup(adj,(word.substr(0, word.length() - 2) + "o").c_str())){
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
         else if(diminutive){
         if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "o").c_str())){
             translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4) + "o").c_str()));

         }else if(lookup(nouns, (word.substr(0, word.length() - 6) + "o").c_str()) ){
             translation = "little " + string(lookup(nouns, (word.substr(0, word.length() - 6) + "o").c_str()));
         }
           else  if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4)  + "a").c_str())){
             translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 4) + "a").c_str()));

         }else  if(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 5)).c_str())){
             translation = "little " + string(lookup(nouns, (script_adequation(word).substr(0, script_adequation(word).length() - 5).c_str())));

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
    
            }
        }
    }
 

    for (size_t i = 0; i < sentence_arr.size(); ++i) {


 

        // ------------------------ PRONOUN ASSIGNING  -----------------
        // english verbs do not conjugate person aside from third vs non-third (and even thats an understatement
        // cause 'they' (3rd plural) falls with 1st. e.g she loves, he loves, you love, i love, we love, they love 
        // point being a portuguese pronoun can infer more info on the person: eu comO, você comE, eles comEM, nos comEMOS.

        // a single verb can define person: vejo -> I see
        if (sentence_arr.size() == 1 && (sentence_arr[0].second == 3 || sentence_arr[0].second == 36 )) {
            reordered_arr.clear(); 
            reordered_arr.push_back(sentence_arr[0]);
            return reordered_arr;
        }

     

        // ------------------------ ADJECTIVE ORDER  -----------------
        // a set is noun[0] and adjective[1], we switch order, so that casa[0] azul[1] -> blue[1] house[0]
        if (i > 0 && sentence_arr[i - 1].second == 0 && sentence_arr[i].second == 1) {
            reordered_arr.pop_back();   

            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 1]); }

        // ------------------------ ARTICLE TWEAKS  --------------------
        // does the next translation start in a vowel? if so the article should be an. a apple -> an apple
        else if (i > 0 && sentence_arr[i - 1].second == 9 && isVowel(sentence_arr[i].first[0])) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i - 1].first + (sentence_arr[i - 1].first != "the" ? "n" : ""), 9});
            reordered_arr.push_back(sentence_arr[i]);  
        }      // intransitive verbs, just plug an "it" at the end, theres way more complicated nuance 
    // but i'm not doing allat now.
    // so if 'eu[2] amo[36]' => 'i[2] (love it[36])]!  
        else  if (sentence_arr[i].second == 36) { 
                bool add_it = true;

                if (i + 1 < sentence_arr.size()) {
                    int next_type = sentence_arr[i + 1].second;
                    if (next_type == 3 || next_type == 36 || next_type == 0 || next_type == 1 || next_type == 4 || next_type == -1 ||  next_type == -2 || next_type == 9) {
                        add_it = false; 
                    }
                }
                reordered_arr.push_back(sentence_arr[i]);

                if (add_it) {
                    reordered_arr.push_back(pair<string, int>{"it", -1});
                }
            }

        // ------------------------ OBLIQUE PRONOUNS  -----------------
        // a set is oblique pronoun[11] and verb[3], we switch order, so that te[11] amo[3] -> love[3] you[11]
        else if (i > 0 && sentence_arr[i - 1].second == 11 && (sentence_arr[i].second == 3 || sentence_arr[i].second == 36)) {
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

        // ------------------------ COMPARATIVE  -----------------
        // a set is word1 = 'mais' and word2 = adj[1], we eliminate the first 'mais' and append 'er' to word2, so that mais forte[2] -> stronger
        else if (i > 0 && sentence_arr[i - 1].first == "more" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i].first + "er", 20});  
        } 

        // ------------------------ INTRANSITIVE VERBS THAT TAKE 'DE'  ----------------- 
        // a set is word1 = verb[3] and word2 = 'de', we eliminate the preposition 'de', so that gosto[1] de  -> stronger
        else if (i > 0 && (sentence_arr[i - 1].second == 3 || sentence_arr[i - 1].second == 36) && sentence_arr[i].first == "of") {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
        } 

        // ------------------------ TRANSITIVE VERBS WITH A PERSONAL PRONOUN  ----------------- 
        // a set is word1 = verb[3] and word2 = pronoun[4], we use the second value of the pair? of the pronoun i guess idk i'm tired
        // like the value of the key 'ela' is a pair<string, string> that holds both the subject and object pronoun {'she', 'her'}
        else if (i > 0 && (sentence_arr[i - 1].second == 3 || sentence_arr[i - 1].second == 36) && sentence_arr[i].second == 4) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);
            reordered_arr.push_back({
              lookup(obj_pro, sentence_arr[i].first.c_str()) 
                  ? string(lookup(obj_pro, sentence_arr[i].first.c_str())) 
                  : sentence_arr[i].first,
              10
          });
        }

        // ------------------------ CONTINUOUS TO BE (IS, ARE, AM)  ----------------- 
        // a set is word1 = pronoun[4] and word2 = "is", use an if to check the first pronoun and change it accordingly 
        // she is, i am, we are
        else if (i > 0 && sentence_arr[i - 1].second == 4 && sentence_arr[i].first == "is") {
            string corr_pro = "is";
            reordered_arr.pop_back(); 
            if(sentence_arr[i - 1].first == "she" || sentence_arr[i - 1].first == "he"){
                corr_pro = "is";
            } else if(sentence_arr[i - 1].first == "we" || sentence_arr[i - 1].first == "they" || sentence_arr[i - 1].first == "you"){
                corr_pro = "are";
            } else if(sentence_arr[i - 1].first == "i"){
                corr_pro = "am";
            };
            reordered_arr.push_back(sentence_arr[i - 1]);   
            reordered_arr.push_back(pair<string, int>{corr_pro, 4});
        } 

        // ------------------------ DOUBLE VERBS ----------------- 
        // a set is verb[3] and verb[3], we add 'to' between them, so that amo[3] correr[3] -> love[3] *to* run[3]
        else if (i > 0 && (sentence_arr[i - 1].second == 3 || sentence_arr[i - 1].second == 36) && (sentence_arr[i].second == 3 || sentence_arr[i].second == 3)) {
            if (sentence_arr[i - 1].first == "don't" || sentence_arr[i - 1].first == "doesn't") {
                reordered_arr.push_back(sentence_arr[i]);
                continue;
            }
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
            if (std::find(modals.begin(), modals.end(), sentence_arr[i - 1].first) == modals.end()) {
                reordered_arr.push_back(pair<string, int>{"to", -1});  
            }
            reordered_arr.push_back(sentence_arr[i]);
        } 

        // same thing but when the verbs are connected by 'que' (e.g: tenho QUE ver)
        else if (i > 1 && (sentence_arr[i - 2].second == 3 || sentence_arr[i - 2].second == 36) && sentence_arr[i - 1].first == "what"  && (sentence_arr[i].second == 3 || sentence_arr[i].second == 36)) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{"to", -1});  
            reordered_arr.push_back(sentence_arr[i]); 
        }

        else if (i > 1 && (sentence_arr[i - 2].second == 3 || sentence_arr[i - 2].second == 3) && sentence_arr[i - 1].first == "of" && (sentence_arr[i].second == 3 || sentence_arr[i].second == 36)) {
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
        // FUCLKKKKKKKKKK THERES ALSO "DOESN'T" FOR THIRD PERSON
        // a set is pronoun[4] + "no"  + verb[3]. 'no' becomes then 'don't' or doesn't so that não* gosto[3] -> don't like instead of 'no like'.
        else if (i > 1 && sentence_arr[i - 2].second == 4 && sentence_arr[i - 1].first == "não" && (sentence_arr[i].second == 3 || sentence_arr[i].second == 36)) {
            string subj = sentence_arr[i - 2].first;
            string verb = sentence_arr[i].first;

            while (!reordered_arr.empty() && (reordered_arr.back().first == subj || reordered_arr.back().first == "não")) {
                reordered_arr.pop_back();
            }

            string aux = (std::find(th_per_aux.begin(), th_per_aux.end(), subj) != th_per_aux.end()) ? "doesn't" : "don't";
            reordered_arr.push_back({subj, 4});
            reordered_arr.push_back({aux, 3});
            reordered_arr.push_back({verb, 3});

            i++; 
        } 

        // ------------------------ é: is or it is? ----------------- 
        // So this seems like the easiest pronoun plug so i'll get it out of the way
        // we just gotta check if is is preceded or not by a pronoun: in this case it should stay 'is'
        // e.g she is, he is, it is, should stay the same
        // if theres no pronoun before it, we have to plug the it. the other pronoun forms will be handled on a different function 
        else if (sentence_arr[i].first == "is") {
          // actualy noun and pronoun lol
            bool preceded_by_pronoun = (i > 0 && sentence_arr[i - 1].second == 4) || (i > 0 && sentence_arr[i - 1].second == 0) ;

            if (preceded_by_pronoun) {
                reordered_arr.push_back({"is", sentence_arr[i].second});
            } else {
                reordered_arr.push_back({"it", 4});
                reordered_arr.push_back({"is", sentence_arr[i].second});
            }
        }

        else {
            reordered_arr.push_back(sentence_arr[i]);
        }
    }

    return reordered_arr;
}


  vector<string> tokenize(const string &text) {
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
              size_t len = 0;
              if ((c & 0xE0) == 0xC0) len = 2;
              else if ((c & 0xF0) == 0xE0) len = 3;
              else if ((c & 0xF8) == 0xF0) len = 4;
              else len = 1;

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
                          firstChar == '.' || firstChar == ','
                          || firstChar == '-' || firstChar == '/' || firstChar == ':');

    if (!sentence.empty() && !isPunctuation) {
        sentence += " ";
    }

    sentence += token;
}
  return sentence;
}
std::string bigramLookup(const std::vector<std::string>& array_of_words, std::vector<int>& ignore_flags) {
    std::vector<std::string> mended_array_of_words;

    size_t i = 1;
    while (i < array_of_words.size()) {
        if (ignore_flags[i-1] != 0) {  // skip already translated word
            mended_array_of_words.push_back(array_of_words[i-1]);
            ignore_flags.push_back(ignore_flags[i-1]);
            i++;
            continue;
        }

        std::string bigram = array_of_words[i-1] + "_" + array_of_words[i];
        const char* bigram_translation = lookup(fixed_ngrams, bigram.c_str());
        if (bigram_translation) {
            mended_array_of_words.push_back(bigram_translation);
            ignore_flags.push_back(1);  
            i += 2;
        } else {
            mended_array_of_words.push_back(array_of_words[i-1]);
            ignore_flags.push_back(0);
            i++;
        }
    }

    if (i == array_of_words.size()) {
        mended_array_of_words.push_back(array_of_words.back());
        ignore_flags.push_back(ignore_flags.back());
    }

    return unigramLookup(mended_array_of_words, ignore_flags);
}

std::string trigramLookup(const std::vector<std::string>& words) {
    std::vector<std::string> mended;
    std::vector<int> ignore_flags;

    size_t i = 0;
    while (i < words.size()) {
        if (i + 2 < words.size()) {
            std::string trigram = words[i] + "_" + words[i+1] + "_" + words[i+2];
            const char* t = lookup(fixed_ngrams, trigram.c_str());
            if (t) {
                mended.push_back(t);
                ignore_flags.push_back(1);
                i += 3;
                continue;
            }
        }
        mended.push_back(words[i]);
        ignore_flags.push_back(0);
        i++;
    }

    return bigramLookup(mended, ignore_flags);

    }
std::string traduzir_en(string sentence) {
    to_lower(&sentence[0]); 
    std::vector<std::string> arr = tokenize(sentence);  
    std::string translated = trigramLookup(arr);
    
    return script_adequation(translated); 
}