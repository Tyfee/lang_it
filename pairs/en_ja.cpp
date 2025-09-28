#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdint.h>

using namespace std;

struct Entry {
    const char* w;
    const uint8_t* t;
    size_t len;
};

struct Verb {
    const char* w;
    const uint8_t* t;
    size_t len;
    int type;
};

enum Kana : uint8_t {
    A = 1, KA, GA, SA, ZA, TA, DA, NA, HA, BA, PA, MA, YA, RA, WA, N,
    I, KI, GI, SI, JI, CHI, NI, HI, BI, PI, MI, RI,
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
    "い","き","ぎ","し","じ","ち","に","ひ","び","ぴ","み","り",
    "う","く","ぐ","す","ず","つ","ぬ","ふ","ぶ","ぷ","む","ゆ","る",
    "え","け","げ","せ","ぜ","て","で","ね","へ","べ","ぺ","め","れ",
    "お","こ","ご","そ","ぞ","と","ど","の","ほ","ぼ","ぽ","も","よ","ろ","を",

    "ア","か","が","さ","ざ","た","だ","な","は","ば","ぱ","ま","や","ら","わ","ン",
    "イ","き","ぎ","し","じ","ち","に","ひ","び","ぴ","み","り",
    "ウ","く","ぐ","す","ず","つ","ぬ","ふ","ぶ","ぷ","む","ゆ","る",
    "エ","け","げ","せ","ぜ","て","で","ね","へ","べ","ぺ","め","れ",
    "オ","こ","ご","そ","ぞ","と","ど","の","ほ","ぼ","ぽ","も","よ","ろ","ヲ"
};

static const uint8_t _i[] = {WA, TA, SI};
static const uint8_t _is[] = {HA};
static const uint8_t _thank[] = {A, RI, GA, TO, U};
static const uint8_t _taberu[] = {TA, BE, RU};
static const uint8_t _tukuru[] = {TU, KU, RU};

constexpr Entry dict[] = { 
    {"i", _i, 3},
    {"am", _is, 1},
    {"are", _is, 1},
    {"is", _is, 1} 

};


constexpr Entry fixed_ngrams[] = { {"thank_you", _thank, 5} };

constexpr Verb verbs[] = { 
    {"eat", _taberu, 3, 1}, 
    {"create", _tukuru, 3, 0} 
};

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

static vector<string> tokenize(const string &text) {
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

static pair<string,int> findVerb(const string &word) {
    const Verb* v = verbLookup(verbs, sizeof(verbs)/sizeof(verbs[0]), word.c_str());
    if (v) {
        string out;
        for (size_t i = 0; i < v->len; ++i) out += HIRA_KATA[v->t[i]];
        return {out, v->type};
    }
    return {word, -1};
}

static pair<string,int> nounLookup(const string &word) {
    const Entry* e = lookup(dict, word.c_str());
    if (e) {
        string out;
        for (size_t i = 0; i < e->len; ++i) out += HIRA_KATA[e->t[i]];
        return {out, 0};
    }
    return findVerb(word);
}

static string unigramLookup(const vector<string> &array_of_words) {
    string sentence;
    for (size_t i = 0; i < array_of_words.size(); ++i) {
        if (!sentence.empty()) sentence += " ";
        auto p = nounLookup(array_of_words[i]);
        sentence += p.first;
    }
    return sentence;
}

static string bigramLookup(const vector<string> &array_of_words) {
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

static string trigramLookup(const vector<string> &array_of_words) {
    return bigramLookup(array_of_words);
}

string translate_ja(string sentence) {
    to_lower(sentence);
    auto tokens = tokenize(sentence);
    return trigramLookup(tokens);
}
