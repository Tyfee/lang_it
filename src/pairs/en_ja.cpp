#include "../lang_it.h"

#if defined(ALL)
#define EN_JA
#endif

#ifdef EN_JA


#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef const int WordJ;

struct EntryJ {
    const char* w;
    const int* t;
    size_t len;
    int kanji[4]; // this will store the offset from the first unicode of CJK (U+4E00), and the server api will spit out the numbers,
    int kanjiCount;
             // whatever frontend being used (preferablly web-javascript) will simply calculate the resulf offset and display the kanji  
               // no kanji will simply send it as it is. the actual hiragana, and will be stored as -1
   int wordType;
   int katakana; // if one, lookup on the katakana enum table
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

struct VerbJ {
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

    JA, CHA, JU, CHU, JO, CHO, NBS, LA, LI, LU, LE, LO, NONE
};



const char* ROMAJI[] = {
    "a","ka","ga","sa","za","ta","da","na","ha","ba","pa","ma","ya","ra","wa","n",
    "i","ki","gi","shi","ji","chi","ni","hi","mi","pi","bi","ri",
    "u","ku","gu","su","zu","tsu","nu","fu","bu","pu","mu","yu","ru",
    "e","ke","ge","se","ze","te","de","ne","he","be","pe","me","re",
    "o","ko","go","so","zo","to","do","no","ho","bo","po","mo","yo","ro","wo",
    "ja","cha","ju","chu","jo","cho", "-", "la", "li", "lu", "le", "lo", ""
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
    "じゃ","ちゃ","じゅ","ちゅ","じょ","ちょ", "ー", "ぁ", "ぃ", "ぅ", "ぇ", "ぉ", ""
};

const string KATAKANA[] = {
    "", "ア","カ","ガ","サ","ザ","タ","ダ","ナ","ハ","バ","パ","マ","ヤ","ラ","ワ","ン",
    "イ","キ","ギ","シ","ジ","チ","ニ","ヒ","ミ","ピ","ビ","リ",
    "ウ","ク","グ","ス","ズ","ツ","ヌ","フ","ブ","プ","ム","ユ","ル",
    "エ","ケ","ゲ","セ","ゼ","テ","デ","ネ","ヘ","ベ","ペ","メ","レ",
    "オ","コ","ゴ","ソ","ゾ","ト","ド","ノ","ホ","ボ","ポ","モ","ヨ","ロ","ヲ",
    "ジャ","チャ","ジュ","チュ","ジョ","チョ", "ー","ァ", "ィ", "ゥ", "ェ", "ォ", ""
};

// wait i'll save some numbers to turn u endings into i endings
// to turn KU into KI, you substract 12, GU into GI, also 12, SHI, SU, 12 AS WELL 0_O

string desu = HIRAGANA[DE] + HIRAGANA[SU];
string masita = HIRAGANA[MA] + HIRAGANA[SI] + HIRAGANA[TA];


WordJ _is[] = {HA};
WordJ _ai[] = {A, I};
WordJ _krs[] = {KO, RO, SU};

WordJ _thank[] = {A, RI, GA, TO, U};
WordJ _taberu[] = {TA, BE, RU};
WordJ _tukuru[] = {TU, KU, RU};
WordJ _egk[] = {E, GA, KU};
WordJ _tmg[] = {TA, MA, GO};

WordJ _important[] = {DA, I, JI};
WordJ _yes[] = {HA, I};
WordJ _ie[] = {I, E};

WordJ _kr[] = {KO, RE};
WordJ _ar[] = {A, RE};

WordJ _kh[] = {KO, NBS, HI, NBS};


WordJ _i[] = {WA, TA, SI};
WordJ _you[] = {A, NA, TA};
WordJ _we[] = {WA, TA, SI, TA, CHI};
WordJ _he[] = {KA, RE};
WordJ _she[] = {KA, NO, JO};
WordJ _they[] = {A, NA, TA, TA, CHI};
WordJ _antn[] = {A, NA, TA, NO};
WordJ _wtsn[] = {WA, TA, SI, NO};
WordJ _to[] = {TO};
WordJ _ski[] = {SE, KA, I};
WordJ _inu[] = {I, NU};
WordJ _tk[] = {TO, KI};
WordJ _knncw[] = {KO, N , NI, CHI, WA};
WordJ _hnyk[] = {HO, N , YA, KU};
WordJ _nn[] = {NA, NI};
WordJ _blank[] = {NONE};




constexpr EntryJ dict[] = { 
    {"and", _to, 1, {-1}, 0 , 3, 0},
    {"am", _is, 1, {-1}, 0 , 3, 0},
    {"are", _is, 1,{-1}, 0, 3, 0},
    {"is", _is, 1,{-1}, 0, 3, 0},   
    {"important", _important, 3, {2855, 139},  2, 1, 0},       // this will be the example of what i'm cooking
                                // pay ATTENTION MATE. an Entry is composed of its 'original_word (important)', 'translation_kana (DA, I, JI)' 
                                // 'size (3)', kanji_offsets {2855, 139}, 'number_of_kanji (2)'
                                // how do you get this shit??? you find the unicode hex value of the kanji you need, and substract the first
                               // character from the CJK ideograms block (4E00), e.g: important => 大事
                               // the hex values are 大 = 5927 and　事 = 4E8B, so we subtract the first char value 
                               // and we get two offsets = B27 and 8B.
                               // then you convert the offset back to decimal and you will get the offset integers (2855, 139)
                               // then your front end needs to make the steps backwards to retrieve the kanji** YOU DONT NEED THAT ANYMORE,
                               // if you pass kanji: true as the translate_ja() param you already get the utf8 ready to display (tested gtk and html)
                               // DO WHAT???????
                               // if theres no kanji associated: the offset is an array with {-1} and the length is 0!.

                               /* heres some js code to find the kanji with the offset
                            
                                function getKanji(offset){
                                let base = 0x4E00;
                                var kanji =  String.fromCodePoint(base + offset);

                                console.log(kanji);
                                }

                                getKanji(1397);

                                </script>
                               
                               
                               */
    
    {"egg", _tmg, 3, {1397},  1, 0, 1},
    {"yes", _yes, 2, {-1}, 0, 0, 0},
    {"thanks", _thank, 5, {-1}, 0, 0, 0},
    {"house", _ie, 2, {3510}, 1, 0, 0},
    {"coffee", _kh, 4, {-1}, 0, 0, 1 /* <- this is katakana */}, 
    {"world", _ski, 3, {22, 10060}, 2, 0, 0},
    {"time", _tk, 3, {6210}, 1, 0, 0},
    {"dog", _inu, 2, {9388}, 1, 0, 0},
    {"hello", _knncw, 5, {-1 }, 0, 0, 0},
    {"translator", _hnyk, 4, {12795, 15411}, 2, 0, 0},
    {"what", _nn, 2, {341}, 1, 4, 0},
    
    {"this", _kr, 2, {-1}, 0, 8, 0},
    {"that", _ar, 2, {-1}, 0, 8, 0},
    {"the", _blank, 0, {-1}, 0, 10, 0},
    {"a", _blank, 0, {-1}, 0, 10, 0},
    {"an", _blank, 0, {-1}, 0, 10, 0},

    // pronouns

    {"i", _i, 3, {11201}, 1, 4, 0},
    {"you", _you, 3, {-1}, 0, 4, 0},
    {"we", _we, 5, {-1}, 0, 4, 0},
    {"he", _he, 2, {4476}, 1, 4, 0},
    {"she", _she, 3, {4476, 2931}, 2, 4, 0},
    {"they", _they, 5, {-1}, 0, 4, 0},
    {"my", _wtsn, 4, {-1}, 0, 44, 0},
    {"your", _antn, 4, {-1}, 0, 44, 0}
};



constexpr EntryJ fixed_ngrams[] = { 
    {"thank_you", _thank, 5, {-1}, 0, 0} 
};

constexpr VerbJ verbs[] = { 
    {"eat", _taberu, 3, 1, 0}, 
    {"create", _tukuru, 3, 0, 0},
    {"love", _ai, 2, 0, 1},
    {"kill", _krs, 3, 0, 0},
    {"draw", _egk, 3, 1, 0}
};

WordJ _ao[] = {A, O};
WordJ _ooki[] = {O, O, KI};
WordJ _chiisai[] = {CHI, I, SA};
WordJ _szk[] = {SI, ZU, KA};
WordJ _mdr[] = {MI, DO, RO};


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

int toAForm(int kana) {
    if (kana >= KU && kana <= RU) {
        switch(kana) {
            case KU: return KA;   
            case GU: return GA;    
            case SU: return SA;    
            case TU: return TA;  
            case NU: return NA;   
            case BU: return BA;    
            case MU: return MA;    
            case RU: return RA;    
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


static const VerbJ* verbLookup(const VerbJ* array, size_t N, const char* word) {
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
    string copy = s;



    size_t pos = 0;
        while ((pos = copy.find("l", pos)) != string::npos) {
            copy.replace(pos, 1, "r");
            pos += 1; 
        }
        pos = 0;
         while ((pos = copy.find("ty", pos)) != string::npos) {
            copy.replace(pos, 3, "tei-");
            pos += 3; 
        }
        pos = 0;
        while ((pos = copy.find("y", pos)) != string::npos) {
            copy.replace(pos, 1, "i");
            pos += 1; 
        }
        
        if (!copy.empty() && copy.back() == 'r') {
            copy = copy.substr(0, copy.length() - 1) + "ru";
        }
     pos = 0;
        while ((pos = copy.find("tra", pos)) != string::npos) {
            copy.replace(pos, 3, "tora");
            pos += 3; 
        }
    pos = 0;
        while ((pos = copy.find("coo", pos)) != string::npos) {
            copy.replace(pos, 3, "ku-");
            pos += 3; 
        }
        pos = 0;
        while ((pos = copy.find("poo", pos)) != string::npos) {
            copy.replace(pos, 3, "pu-");
            pos += 3; 
        }
         pos = 0;
        while ((pos = copy.find("par", pos)) != string::npos) {
            copy.replace(pos, 3, "pa-");
            pos += 3; 
        }
        pos = 0;
        while ((pos = copy.find("tra", pos)) != string::npos) {
            copy.replace(pos, 3, "tora");
            pos += 3; 
        }
          pos = 0;
        while ((pos = copy.find("pro", pos)) != string::npos) {
            copy.replace(pos, 3, "puro");
            pos += 3; 
        }
           pos = 0;
        while ((pos = copy.find("pre", pos)) != string::npos) {
            copy.replace(pos, 3, "pure");
            pos += 3; 
        }
        
    

    size_t i = 0;

    while (i < copy.size()) {
        Kana k = static_cast<Kana>(0);
        string chunk;
        if (i + 2 < copy.size()) {
            chunk = copy.substr(i, 3);
            k = kanaFromRomaji(chunk);
        }
        if (static_cast<int>(k) == 0 && i + 1 < copy.size()) {
            chunk = copy.substr(i, 2);
            k = kanaFromRomaji(chunk);
        }
        if (static_cast<int>(k) == 0) {
            chunk = copy.substr(i, 1);
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


vector<Result> reorder_helpers(vector<Result> &array_of_words, int script) {
vector<Result> array_of_results = array_of_words;
 

    array_of_results.push_back(Result{" ", script == 2 ? "desu" : desu, -1});
    if(array_of_words.back().word == "?"){
        array_of_words.pop_back();
        
        
    array_of_results.push_back(Result{" ", script == 2 ? "ka" : HIRAGANA[KA], -1});
    } 
    if(array_of_words.back().word == "?" && script == 2){
        array_of_words.pop_back();
        array_of_results.push_back(Result{"ka"," ka",-1});
             
    }

    for(int i = 0; i < array_of_words.size(); ++i){
        if(i > 0 && array_of_words[i - 1].type == 3 && array_of_words[i].type == 4){
            array_of_results.pop_back();  array_of_results.pop_back();
             array_of_results.push_back(Result{array_of_words[i].word,array_of_words[i].translation,array_of_words[i].type});
            array_of_results.push_back(Result{"wo",HIRAGANA[WO],-1});
             
             array_of_results.push_back(Result{array_of_words[i -1].word,array_of_words[i - 1].translation,array_of_words[i - 1].type});
        }
    };

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
    const VerbJ* v = verbLookup(verbs, sizeof(verbs)/sizeof(verbs[0]), word.c_str());

    const VerbJ* v_past_e = nullptr;
    const VerbJ* v_past_n = nullptr;

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
        const VerbJ* vref = v_past_e ? v_past_e : v_past_n;
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

std::string getKanjiFromOffset(int offset) {
    char32_t base = 0x4E00;
    char32_t ch = base + offset;

    std::string out;
    if (ch <= 0x7F) {
        out += static_cast<char>(ch);
    } else if (ch <= 0x7FF) {
        out += static_cast<char>(0xC0 | ((ch >> 6) & 0x1F));
        out += static_cast<char>(0x80 | (ch & 0x3F));
    } else if (ch <= 0xFFFF) {
        out += static_cast<char>(0xE0 | ((ch >> 12) & 0x0F));
        out += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (ch & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | ((ch >> 18) & 0x07));
        out += static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (ch & 0x3F));
    }
    return out;
}
static Result nounLookup(string &word, int script) {
    string translation;
    int word_type;
    const EntryJ* e = lookup(dict, word.c_str());
    const EntryJ* e_p = lookup(dict, word.substr(0, word.length() - 1).c_str()); //plural nouns
    const Adjective* a = lookup(adj, word.c_str());
    
    Result vres = findVerb(word);


    // we can now decide which one to receive O_0
    // i'll just have to impleemnt romaji and kanji as query params and function params or whatever
    string out;
    string romaji;
    if (e) {


    for (size_t i = 0; i < e->len; ++i) {
        if (script == 1) {
            if (e->kanjiCount > 0) {
                for (int k = 0; k < e->kanjiCount; ++k) {
                    if (e->kanji[k] != -1) {
                        out += getKanjiFromOffset(e->kanji[k]);
                        i += e->len;
                    } else {
                        out += e->katakana == 0 ? HIRAGANA[e->t[i]] : KATAKANA[e->t[i]];
                    }
                }
            } else {
                out += e->katakana == 0 ? HIRAGANA[e->t[i]] : KATAKANA[e->t[i]];
            }
        } else {
            out += e->katakana == 0 ? HIRAGANA[e->t[i]] : KATAKANA[e->t[i]];
        }
    }

        for (size_t i = 0; i < e->len; ++i){
            romaji += ROMAJI[e->t[i] - 1];
        }
        translation = script == 2 ? (romaji + " ") : out;
        word_type = 0;
         
    }else  if (e_p) {


    for (size_t i = 0; i < e_p->len; ++i) {
        if (script == 2) {
            if (e_p->kanjiCount > 0) {
                for (int k = 0; k < e_p->kanjiCount; ++k) {
                    if (e_p->kanji[k] != -1) {
                        out += getKanjiFromOffset(e_p->kanji[k]);
                        i += e_p->len;
                    } else {
                        out += HIRAGANA[e_p->t[i]];
                    }
                }
            } else {
                out += HIRAGANA[e_p->t[i]];
            }
        } else {
            out += HIRAGANA[e_p->t[i]];
        }
    }

        for (size_t i = 0; i < e_p->len; ++i) romaji += ROMAJI[e_p->t[i] - 1];
        
        translation = script == 2 ? romaji : out;
        word_type = 0;
         
    }else if(a){
        for (size_t i = 0; i < a->len; ++i) out += HIRAGANA[a->t[i]];
        if(a->type == 0) out += HIRAGANA[I];
        if(a->type == 1) out += HIRAGANA[NA];
        translation = out;
        word_type = 1;
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

static string unigramLookup(vector<string> &array_of_words, int script) {
    string sentence;
    vector<Result> array_of_results;

    for (size_t i = 0; i < array_of_words.size(); ++i) {
        if (!sentence.empty()) sentence += " ";
        auto p = nounLookup(array_of_words[i], script);
        array_of_results.push_back(p);
    }
    array_of_results = reorder_helpers(array_of_results, script);
    for(int i = 0; i< array_of_results.size(); ++i) sentence += array_of_results[i].translation;
    return sentence;
}

static string bigramLookup(vector<string> &array_of_words, int script) {
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
    return unigramLookup(result, script);
}

static string trigramLookup(vector<string> &array_of_words, int script) {
    
    return bigramLookup(array_of_words, script);
}

std::string translate_ja(const char* sentence, int script) {
    std::string s(sentence);  
    to_lower(s);            
    auto tokens = tokenize(s);
    return trigramLookup(tokens, script);
}

#endif