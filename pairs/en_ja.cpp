#define EN_JA
#include "../lang_it.h"

#ifdef EN_JA


#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef const int Word;

struct Entry {
    const char* w;
    const int* t;
    size_t len;
};

struct Adjective {
    const char* w;
    const int* t;
    size_t len;
    int type; // 0 == i, 1 == na
};

typedef struct {
   string word;
   string translation;
   int type;
} Result;

struct Verb {
    const char* w;
    const int* t;
    size_t len;
    int type;
    int compound; // 0 = 作「る」 1 =　愛「する」
};

enum Kana : int {
    A = 1, KA, GA, SA, ZA, TA, DA, NA, HA, BA, PA, MA, YA, RA, WA, N,
    I, KI, GI, SI, JI, CHI, NI, HI, MI, PI, BI, RI, 
    U, KU, GU, SU, ZU, TU, NU, HU, BU, PU, MU, YU, RU,
    E, KE, GE, SE, ZE, TE, DE, NE, HE, BE, PE, ME, RE,
    O, KO, GO, SO, ZO, TO, DO, NO, HO, BO, PO, MO, YO, RO, WO,

    JA, CHA, JU, CHU, JO, CHO, NBS
};



const char* ROMAJI[] = {
    "a","ka","ga","sa","za","ta","da","na","ha","ba","pa","ma","ya","ra","wa","n",
    "i","ki","gi","shi","ji","chi","ni","hi","mi","pi","bi","ri",
    "u","ku","gu","su","zu","tsu","nu","fu","bu","pu","mu","yu","ru",
    "e","ke","ge","se","ze","te","de","ne","he","be","pe","me","re",
    "o","ko","go","so","zo","to","do","no","ho","bo","po","mo","yo","ro","wo",
    "ja","cha","ju","chu","jo","cho", "-"
};

Kana kanaFromRomaji(const std::string &r) {
    for (int k = 1; k <= 66; ++k) {
        if (r == ROMAJI[k - 1]) return static_cast<Kana>(k);  // Note: k-1
    }
    return static_cast<Kana>(0); 
}

const string HIRAGANA[] = {
    "", "あ","か","が","さ","ざ","た","だ","な","は","ば","ぱ","ま","や","ら","わ","ん",
    "い","き","ぎ","し","じ","ち","に","ひ","み","ぴ","び","り",
    "う","く","ぐ","す","ず","つ","ぬ","ふ","ぶ","ぷ","む","ゆ","る",
    "え","け","げ","せ","ぜ","て","で","ね","へ","べ","ぺ","め","れ",
    "お","こ","ご","そ","ぞ","と","ど","の","ほ","ぼ","ぽ","も","よ","ろ","を",
    "じゃ","ちゃ","じゅ","ちゅ","じょ","ちょ", "ー"
};

const string KATAKANA[] = {
    "", "ア","カ","ガ","サ","ザ","タ","ダ","ナ","ハ","バ","パ","マ","ヤ","ラ","ワ","ン",
    "イ","キ","ギ","シ","ジ","チ","ニ","ヒ","ミ","ピ","ビ","リ",
    "ウ","ク","グ","ス","ズ","ツ","ヌ","フ","ブ","プ","ム","ユ","ル",
    "エ","ケ","ゲ","セ","ゼ","テ","デ","ネ","ヘ","ベ","ペ","メ","レ",
    "オ","コ","ゴ","ソ","ゾ","ト","ド","ノ","ホ","ボ","ポ","モ","ヨ","ロ","ヲ",
    "ジャ","チャ","ジュ","チュ","ジョ","チョ", "ー"
};

// wait i'll save some numbers to turn u endings into i endings
// to turn KU into KI, you substract 12, GU into GI, also 12, SHI, SU, 12 AS WELL 0_O

string desu = HIRAGANA[DE] + HIRAGANA[SU];
string masita = HIRAGANA[MA] + HIRAGANA[SI] + HIRAGANA[TA];


Word _is[] = {HA};
Word _ai[] = {A, I};
Word _krs[] = {KO, RO, SU};

Word _thank[] = {A, RI, GA, TO, U};
Word _taberu[] = {TA, BE, RU};
Word _tukuru[] = {TU, KU, RU};
Word _egk[] = {E, GA, KU};

Word _important[] = {DA, I, JI};
Word _yes[] = {HA, I};
Word _ie[] = {I, E};
Word _kh[] = {KO, NBS, HI, NBS};
constexpr Entry dict[] = { 
    {"am", _is, 1},
    {"are", _is, 1},
    {"is", _is, 1},
    {"important", _important, 3},
    {"yes", _yes, 2},
    {"thanks", _thank, 3},
    {"house", _ie, 2},
    {"coffee", _kh, 4}
};


Word _i[] = {WA, TA, SI};
Word _you[] = {A, NA, TA};
Word _we[] = {WA, TA, SI, TA, CHI};
Word _he[] = {KA, RE};
Word _she[] = {KA, NO, JO};
Word _they[] = {A, NA, TA, TA, CHI};
Word _antn[] = {A, NA, TA, NO};
Word _wtsn[] = {WA, TA, SI, NO};

constexpr Entry pro[] = { 
    {"i", _i, 3},
    {"you", _you, 3},
    {"we", _we, 5},
    {"he", _he, 2},
    {"she", _she, 3},
    {"they", _they, 5},
    {"my", _wtsn, 4},
    {"your", _antn, 4}
};



constexpr Entry fixed_ngrams[] = { 
    {"thank_you", _thank, 5} 
};

constexpr Verb verbs[] = { 
    {"eat", _taberu, 3, 1, 0}, 
    {"create", _tukuru, 3, 0, 0},
    {"love", _ai, 2, 0, 1},
    {"kill", _krs, 3, 0, 0},
    {"draw", _egk, 3, 1, 0}
};

Word _ao[] = {A, O};
Word _ooki[] = {O, O, KI};
Word _chiisai[] = {CHI, I, SA};
Word _szk[] = {SI, ZU, KA};
Word _mdr[] = {MI, DO, RO};


constexpr Adjective adj[] = { 
    {"blue", _ao, 2, 0}, 
    {"big", _ooki, 3, 0},
    {"small", _chiisai, 3, 0},
    {"quiet", _szk, 3, 1},
    {"green", _mdr, 3, 1}
};

int toIForm(int kana) {
    if (kana >= KU && kana <= RU) {
        switch(kana) {
            case KU: return KI;   
            case GU: return GI;    
            case SU: return SI;    
            case TU: return CHI;  
            case NU: return NI;   
            case BU: return BI;    
            case MU: return MI;    
            case RU: return RI;    
            default: return kana;
        }
    }
    return kana; 
}

template <typename T, size_t N>
static const T* lookup(const T (&array)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = array[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) ++p, ++q;
        if (*p == *q) return &array[i];
    }
    return nullptr;
}


static const Verb* verbLookup(const Verb* array, size_t N, const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = array[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) ++p, ++q;
        if (*p == *q) return &array[i];
    }
    return nullptr;
}

static void to_lower(string &s) {
    for (char &c : s) if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
}
string script_adequation(const string &s) {
    string converted;
    size_t i = 0;

    while (i < s.size()) {
        Kana k = static_cast<Kana>(0);
        string chunk;
        if (i + 2 < s.size()) {
            chunk = s.substr(i, 3);
            k = kanaFromRomaji(chunk);
        }
        if (static_cast<int>(k) == 0 && i + 1 < s.size()) {
            chunk = s.substr(i, 2);
            k = kanaFromRomaji(chunk);
        }
        if (static_cast<int>(k) == 0) {
            chunk = s.substr(i, 1);
            k = kanaFromRomaji(chunk);
        }
        if (static_cast<int>(k) != 0) {
            converted += KATAKANA[static_cast<int>(k)]; 
        }else {
            converted += chunk; 
        }

        i += chunk.size();
    }

    return converted;
}


vector<Result> reorder_helpers(vector<Result> &array_of_words) {
vector<Result> array_of_results = array_of_words;

    if(array_of_words.back().word == "?"){
        array_of_words.pop_back();
        desu += HIRAGANA[KA];
    }

    for(int i = 0; i < array_of_words.size(); ++i){
        if(i > 0 && array_of_words[i - 1].type == 3 && array_of_words[i].type == 4){
            array_of_results.pop_back();  array_of_results.pop_back();
             array_of_results.push_back(Result{array_of_words[i].word,array_of_words[i].translation,array_of_words[i].type});
            array_of_results.push_back(Result{"wo",HIRAGANA[WO],-1});
             
             array_of_results.push_back(Result{array_of_words[i -1].word,array_of_words[i - 1].translation,array_of_words[i - 1].type});
        }
    };

    array_of_results.push_back(Result{" ", desu, -1});
return array_of_results;
}

static vector<string> tokenize(string &text) {
    vector<string> tokens;
    string current;
    size_t i = 0;
    while (i < text.size()) {
        unsigned char c = text[i];
        if ((c & 0x80) == 0) {
            if (isalnum(c)) current += c;
            else {
                if (!current.empty()) { tokens.push_back(current); current.clear(); }
                if (!isspace(c)) tokens.push_back(string(1, c));
            }
            ++i;
        } else {
            size_t len = 0;
            if ((c & 0xE0) == 0xC0) len = 2;
            else if ((c & 0xF0) == 0xE0) len = 3;
            else if ((c & 0xF8) == 0xF0) len = 4;
            else len = 1;
            current += text.substr(i, len);
            i += len;
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}
static Result findVerb(string &word) {
    const Verb* v = verbLookup(verbs, sizeof(verbs)/sizeof(verbs[0]), word.c_str());

    const Verb* v_past_e = nullptr;
    const Verb* v_past_n = nullptr;

    if (word.size() > 2) {
        string base_ed = word.substr(0, word.size() - 2);
        v_past_e = verbLookup(verbs, sizeof(verbs)/sizeof(verbs[0]), base_ed.c_str());
    }

    if (word.size() > 1) {
        string base_d = word.substr(0, word.size() - 1);
        v_past_n = verbLookup(verbs, sizeof(verbs)/sizeof(verbs[0]), base_d.c_str());
    }

    bool v_past = v_past_e || v_past_n;

    if (v) {
        string out;
        for (size_t i = 0; i < v->len; ++i)
            out += HIRAGANA[v->t[i]];
             if(v->compound == 0){

                return Result{word, out, 3};
             }else{
                 return Result{word, out +  HIRAGANA[SU] + HIRAGANA[RU], 3};
             }
    } 
    else if (v_past) {
        const Verb* vref = v_past_e ? v_past_e : v_past_n;
        string out;
        for (size_t i = 0; i < vref->len; ++i)
            out += HIRAGANA[vref->t[i]];
        if(vref->compound == 1){
           return Result{word, out + HIRAGANA[SI] + masita, 31};
        }else{
              int i_form = toIForm(vref->t[vref->len - 1]);

           return Result{word, out.substr(0, out.length() - 3) +  HIRAGANA[i_form]  + masita, 3};
        }
    }

    return Result{word, "", -1};
}


static Result nounLookup(string &word) {
    string translation;
    int word_type;
    const Entry* e = lookup(dict, word.c_str());
    const Entry* p = lookup(pro, word.c_str());
    const Adjective* a = lookup(adj, word.c_str());
    Result vres = findVerb(word);


    string out;
    if (e) {
        for (size_t i = 0; i < e->len; ++i) out += HIRAGANA[e->t[i]];
        translation = out;
        word_type = 0;
         
    }else if(a){
        for (size_t i = 0; i < a->len; ++i) out += HIRAGANA[a->t[i]];
        if(a->type == 0) out += HIRAGANA[I];
        if(a->type == 1) out += HIRAGANA[NA];
        translation = out;
        word_type = 1;
    }else if(p){
        for (size_t i = 0; i < p->len; ++i) out += HIRAGANA[p->t[i]];
        translation = out;
        word_type = 4;
    }
    else if (vres.translation.size() > 0) {
        translation = vres.translation;
        word_type = 3;
    }else{
        translation = script_adequation(word);
        word_type = -1;
    }
 return Result{word, translation, word_type};
}

static string unigramLookup(vector<string> &array_of_words) {
    string sentence;
    vector<Result> array_of_results;

    for (size_t i = 0; i < array_of_words.size(); ++i) {
        if (!sentence.empty()) sentence += " ";
        auto p = nounLookup(array_of_words[i]);
        array_of_results.push_back(p);
    }
    array_of_results = reorder_helpers(array_of_results);
    for(int i = 0; i< array_of_results.size(); ++i) sentence += array_of_results[i].translation;
    return sentence;
}

static string bigramLookup(vector<string> &array_of_words) {
    vector<string> result;
    size_t i = 1;
    while (i < array_of_words.size()) {
        string bigram = array_of_words[i-1] + "_" + array_of_words[i];
        if (auto match = lookup(fixed_ngrams, bigram.c_str())) {
            string out;
            for (size_t j = 0; j < match->len; ++j) out += HIRAGANA[match->t[j]];
            result.push_back(out);
            i += 2;
        } else {
            result.push_back(array_of_words[i-1]);
            i += 1;
        }
    }
    if (i == array_of_words.size()) result.push_back(array_of_words.back());
    return unigramLookup(result);
}

static string trigramLookup(vector<string> &array_of_words) {
    
    return bigramLookup(array_of_words);
}

std::string translate_ja(const char* sentence) {
    std::string s(sentence);  
    to_lower(s);            
    auto tokens = tokenize(s);
    return trigramLookup(tokens);
}

#endif