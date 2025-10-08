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
    I, KI, GI, SI, JI, CHI, NI, HI, BI, PI, MI, HH, RI,
    U, KU, GU, SU, ZU, TU, NU, HU, BU, PU, MU, YU, RU,
    E, KE, GE, SE, ZE, TE, DE, NE, HE, BE, PE, ME, RE,
    O, KO, GO, SO, ZO, TO, DO, NO, HO, BO, PO, MO, YO, RO, WO,
   
    a, ka, ga, sa, za, ta, da, na, ha, ba, pa, ma, ya, ra, wa, n,
    i, ki, gi, si, ji, chi, ni, hi, bi, pi, mi, ri,
    u, ku, gu, su, zu, tu, nu, hu, bu, pu, mu, yu, ru,
    e, ke, ge, se, ze, te, de, ne, he, be, pe, me, re,
    o, ko, go, so, zo, to, _do, no, ho, bo, po, mo, yo, ro, wo
};

const string HIRA_KATA[] = {
    "", "あ","か","が","さ","ざ","た","だ","な","は","ば","ぱ","ま","や","ら","わ","ん",
    "い", "き","ぎ","し","じ","ち","に","ひ","び","ぴ","み", "", "り",
    "う","く","ぐ","す","ず","つ","ぬ","ふ","ぶ","ぷ","む","ゆ","る",
    "え","け","げ","せ","ぜ","て","で","ね","へ","べ","ぺ","め","れ",
    "お","こ","ご","そ","ぞ","と","ど","の","ほ","ぼ","ぽ","も","よ","ろ","を",

    "ア","か","が","さ","ざ","た","だ","な","は","ば","ぱ","ま","や","ら","わ","ン",
    "イ","き","ぎ","し","じ","ち","に","ひ","び","ぴ","み","り",
    "ウ","く","ぐ","す","ず","つ","ぬ","ふ","ぶ","ぷ","む","ゆ","る",
    "エ","け","げ","せ","ぜ","て","で","ね","へ","べ","ぺ","め","れ",
    "オ","こ","ご","そ","ぞ","と","ど","の","ほ","ぼ","ぽ","も","よ","ろ","ヲ"
};

// wait i'll save some numbers to turn u endings into i endings
// to turn KU into KI, you substract 12, GU into GI, also 12, SHI, SU, 12 AS WELL 0_O

string desu = HIRA_KATA[DE] + HIRA_KATA[SU];
string masita = HIRA_KATA[MA] + HIRA_KATA[SI] + HIRA_KATA[TA];


Word _i[] = {WA, TA, SI};
Word _is[] = {HA};
Word _ai[] = {A, I};
Word _krs[] = {KO, RO, SU};

Word _thank[] = {A, RI, GA, TO, U};
Word _taberu[] = {TA, BE, RU};
Word _tukuru[] = {TU, KU, RU};
Word _egk[] = {E, GA, KU};

Word _important[] = {DA, I, JI};
Word _yes[] = {HA, I};

constexpr Entry dict[] = { 
    {"i", _i, 3},
    {"am", _is, 1},
    {"are", _is, 1},
    {"is", _is, 1},
    {"important", _important, 3},
    {"yes", _yes, 2},
    {"thanks", _thank, 3}

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

int toIForm(int kana) {
    if (kana >= KU && kana <= RU) return kana - 13;
    return kana; // otherwise return unchanged
}

template <size_t N>
static const Entry* lookup(const Entry (&array)[N], const char* word) {
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

vector<Result> reorder_helpers(vector<Result> &array_of_words) {
vector<Result> array_of_results = array_of_words;

    if(array_of_words.back().word == "?"){
        array_of_words.pop_back();
        desu += HIRA_KATA[KA];
    }
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
            out += HIRA_KATA[v->t[i]];
        return Result{word, out, 3};
    } 
    else if (v_past) {
        const Verb* vref = v_past_e ? v_past_e : v_past_n;
        string out;
        for (size_t i = 0; i < vref->len; ++i)
            out += HIRA_KATA[vref->t[i]];
        if(vref->compound == 1){
           return Result{word, out + HIRA_KATA[SI] + masita, 3};
        }else{
              int i_form = toIForm(vref->t[vref->len - 1]);

           return Result{word, out.substr(0, out.length() - 3) +  HIRA_KATA[i_form]  + masita, 3};
        }
    }

    return Result{word, " ", -1};
}


static Result nounLookup(string &word) {
    const Entry* e = lookup(dict, word.c_str());
    if (e) {
        string out;
        for (size_t i = 0; i < e->len; ++i) out += HIRA_KATA[e->t[i]];
       
        return Result{word, out, -1};
    }
    return findVerb(word);
}

static string unigramLookup(vector<string> &array_of_words) {
    string sentence;
    vector<Result> array_of_results;

    for (size_t i = 0; i < array_of_words.size(); ++i) {
        if (!sentence.empty()) sentence += " ";
        auto p = nounLookup(array_of_words[i]);
        array_of_results.push_back(Result{p.word, p.translation, 0});
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
            for (size_t j = 0; j < match->len; ++j) out += HIRA_KATA[match->t[j]];
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