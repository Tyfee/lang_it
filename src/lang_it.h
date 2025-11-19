#ifndef LANG_IT_H
#define LANG_IT_H

#define RULE(str) \
    if (rule(str, sentence_arr, reordered_arr, i)) continue;


#define INVERT(FIRST, SECOND) \
    if ((i >= 1) && (&sentence_arr.at(i - 1))->type == FIRST &&  sentence_arr.at(i).type == SECOND) { \
        invert(reordered_arr, sentence_arr.at(i), sentence_arr.at(i - 1)); \
        continue; \
    }



#include <string>
#include <cstdint>
#include <vector>
#include <iostream>

// pairs

#if defined(PT_EN) || defined(ALL) 
std::string traduzir_en(const char* sentence);
#endif

#if defined(PT_ES) || defined(ALL)
std::string traduzir_es(const char* sentence);
#endif

#if defined(EN_JA) || defined(ALL)
std::string translate_ja(const char* sentence, int script);
#endif

#if defined(EN_PT) || defined(ALL)
std::string translate_pt(const char* sentence);
#endif

#if defined(PT_SV) || defined(ALL)
std::string traduzir_sv(const char* sentence);
#endif





/* ------- GLOBAL CORE FUNCTIONS -----------
|           all pairs use                  |  
------------------------------------------*/

enum WordType {
    NOUN = 0,
    ADJECTIVE = 1,
    VERB = 3,
    INTRANSITIVE_VERB = 36,
    PRONOUN = 4,
    OBLIQUE_PRONOUN = 11,
    PREPOSITION = 8,
    ARTICLE = 9,
    ADVERB = 13,
    POSSESSIVE_PRONOUN = 40
};

inline WordType typeFromString(const std::string& s) {
    if (s == "NOUN") return NOUN;
    if (s == "ADJECTIVE") return ADJECTIVE;
    if (s == "VERB") return VERB;
    if (s == "INTRANSITIVE_VERB") return INTRANSITIVE_VERB;
    if (s == "PRONOUN") return PRONOUN;
    if (s == "OBLIQUE_PRONOUN") return OBLIQUE_PRONOUN;
    if (s == "PREPOSITION") return PREPOSITION;
    if (s == "ARTICLE") return ARTICLE;
    if (s == "ADVERB") return ADVERB;
    if (s == "POSSESSIVE_PRONOUN") return POSSESSIVE_PRONOUN;

    return NOUN;
}



enum Flags: uint8_t {
    ANIMATE = 0,
    IS_HUMAN = 1 << 0,
    NO_PLURAL = 1 << 1,
    IRREGULAR_PLURAL = 1 << 2,
    IS_PLACE = 1 << 3,
    ON = 1 << 4, // should use ON instead of IN
    UNCOUNTABLE = 1 << 5,
    FEMININE_NEUTER = 1 << 6, // need that // will call that the FEMININE_NEUTER so that swedish works too, don't know what i'd do for three-gendered languages
    NOT_GENDERED = 1 << 7 // need that as well
                            // MAYBE if something is FEMININE_NEUTER AND NOT_GENDERED SIMULTANEOUSLY i'll consider it the third neutral gender
};

enum VerbFlags: uint8_t {
    REFLEXIVE = 0,
    INTRANSITIVE = 1 << 0,
    DATIVE_CONST = 1 << 1
};

enum SuffixFlags: uint8_t {
    PLURAL = 0,
    FEMININE = 1 << 0
};

typedef struct {
    const char* w;
    const char* t;
    uint8_t flags;
} Entry;


typedef struct {
   std::string word;
   std::string translation;
   int type;
} Word;



struct Verb {
    const char* root;       
    const char* translation; 
    int type;      
    uint8_t flags;
};


typedef struct
{
  const char* w;
  const char* t;
  int type;
  uint8_t flags;
} Suffix;



// making a decent API for the pairs to access and make implementing easier, but i know i'll keep changing it and never be satisfied
// also, the reason as to why the flag lookups functions are all separate is because i implemented bit masking after having most of the shit done
// i could return the whole struct and access jjust waht i need but i'm sooooooooooooooooooooo lazy.
template <size_t N>
inline const char* lookup(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].t;
    }
    return nullptr;
}

template <size_t N>
inline uint8_t lookupFlags(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}
template <size_t N>
inline uint8_t lookupVerbFlags(const Verb (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].root;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}


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
    return Suffix{nullptr, nullptr, 99, 0};
}

template <size_t N>
inline uint8_t lookupSuffFlags(const Suffix (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].root;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].flags;
    }
    return 0;
}





//invert a pair (casa azul -> azul casa)
inline void invert(std::vector<Word>& output, const Word& first, const Word& second) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(second);
}
//
inline void replace_first(std::vector<Word>& output, const Word& replacement, const Word& second) {
    if (!output.empty()) output.pop_back();

    output.push_back(replacement);
    output.push_back(second);
}

// inject a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich(std::vector<Word>& output, const Word& first, const Word& middle, const Word& second) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(first);
    output.push_back(middle);
    output.push_back(second);
}

// replace a word in the middle of two words (orange juice -> suco de laranja)
inline void sandwich_replace(std::vector<Word>& output,const Word& middle, const Word& second) {
    if (!output.empty()) output.pop_back(); 
    output.push_back(middle);
    output.push_back(second);
}

//remove middle word
inline void remove_middle(std::vector<Word>& output, const Word& first, const Word& middle, const Word& last) {
    if (!output.empty()) output.pop_back();
    output.push_back(first);
    output.push_back(last);
}
inline void remove_previous(std::vector<Word>& output) {
    output.erase(output.end() - 2);  
}

inline void remove_current(std::vector<Word>& output) {
    output.pop_back();            
}

inline void remove_pair(std::vector<Word>& output) {
    if (output.size() >= 2) {
        output.erase(output.end() - 2, output.end()); 
    }
}

// all the lookups

using Reorder = std::vector<Word>(*)(const std::vector<Word>&);
using NounLookup = Word(*)(const std::string&);

//ngram groups
inline std::string unigramLookup(const std::vector<std::string>& array_of_words,
                                 const std::vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr){

  std::vector<Word> sentence_arr;
  std::vector<Word> word_arr;

  int match_type;
  std::string sentence;
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
  if(word_arr.size() > 0) sentence_arr = reorder_helpers(word_arr);

  
 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const std::string& token = sentence_arr.at(i).translation;

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

template <size_t N>
inline std::string bigramLookup(const Entry (&fixed_ngrams)[N],
                                const std::vector<std::string>& words,
                                std::vector<int>& ignore_flags, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr) {
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

    return unigramLookup(mended_array_of_words, new_ignore_flags, reorder_helpers, nounLookup);
}


template <size_t N>
inline std::string trigramLookup(const Entry (&fixed_ngrams)[N],
                                 const std::vector<std::string>& words, Reorder reorder_helpers = nullptr, NounLookup nounLookup = nullptr) { 
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
   return bigramLookup(fixed_ngrams, mended, ignore_flags, reorder_helpers, nounLookup);
}


// Y is considered a vowel for english reasons obviously but one day i'll see what to do, but only
// if another language i implement needs a vowel as a consonant
inline bool isVowel(char x)
{
    if (x == 'a' || x == 'e' || x == 'i' || x == 'o'
        || x == 'u' || x == 'y' || x == 'A' || x == 'E' || x == 'I'
        || x == 'O' || x == 'U' || x == 'Y')
    return true;
    else
     return false;
}





inline void to_lower(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
}


inline std::vector<std::string> tokenize(const std::string &text) {
      std::vector<std::string> tokens;
      std::string current;
      size_t i = 0;

      while (i < text.size()) {
          unsigned char c = text[i];

          if ((c & 0x80) == 0) {
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

  

inline std::vector<std::string> tokenize_cjk(const std::string &text) {
    std::vector<std::string> tokens;
    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = text[i];
        size_t len = 0;

        // Determine UTF-8 sequence length
        if ((c & 0x80) == 0) {
            // ASCII
            len = 1;
        } else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        else len = 1; // fallback, invalid byte

        std::string utf8char = text.substr(i, len);
        tokens.push_back(utf8char);

        i += len;
    }

    return tokens;
}





inline bool isPunctuation(const std::string &token) {
    if (token.empty()) return false;

    const std::string punct = ".,?!-/:;()[]{}\"'";

    char first = token.front();
    char last  = token.back();

    return punct.find(first) != std::string::npos || punct.find(last) != std::string::npos;
}

inline uint32_t next_utf8_char(const std::string& s, size_t& i) {
    unsigned char c = s[i];
    uint32_t cp = 0;
    size_t extra = 0;

    if (c <= 0x7F) {
        cp = c;
        extra = 0;
    } else if ((c & 0xE0) == 0xC0) { 
        cp = c & 0x1F;
        extra = 1;
    } else if ((c & 0xF0) == 0xE0) { 
        cp = c & 0x0F;
        extra = 2;
    } else if ((c & 0xF8) == 0xF0) {
        cp = c & 0x07;
        extra = 3;
    } else {
        i++; 
        return 0xFFFD; 
    }

    for (size_t j = 1; j <= extra; ++j) {
        if (i + j >= s.size()) return 0xFFFD;
        unsigned char cc = s[i + j];
        if ((cc & 0xC0) != 0x80) return 0xFFFD;
        cp = (cp << 6) | (cc & 0x3F);
    }

    i += extra;
    return cp;
}

struct Action {
    const char* key;
    void (*fp)(std::vector<Word>&, const Word&, const Word&);
};


inline Action actions[] = {
    { "INVERT", &invert }
};


inline std::vector<std::string> parser(const std::string& s) {
    std::vector<std::string> out;
    std::string current;

    for (char c : s) {
        if (c == ' ') {
            if (!current.empty()) {
                out.push_back(current);
                current.clear();
            }
        } else {
            current.push_back(c);
        }
    }

    if (!current.empty()) {
        out.push_back(current);
    }

    return out;
}


inline auto lookupFunction(const char* query)
    -> void (*)(std::vector<Word>&, const Word&, const Word&)
{
    int actionCount = sizeof(actions) / sizeof(actions[0]);

    for (int i = 0; i < actionCount; i++) {

        const char* p = query;
        const char* r = actions[i].key;
        while (*p && *r && (*p == *r)) {
            p++;
            r++;
        }
        if (*p == *r) {
            return actions[i].fp;
        }
    }

    return nullptr;
}
inline bool rule(
    const std::string& rule,
    const std::vector<Word>& sentence_arr,
    std::vector<Word>& reordered_arr,
    int i
){
    std::vector<std::string> t = parser(rule);
   
    // this accounts for IF A THEN B(C..D..) DO ACTION
    //its valud if theres more than 6 and starts in IF
    if (t.size() < 6) return false;
    if (t[0] != "IF") return false;

    // -first parse a
    std::string A_str = t[1];
    WordType A = typeFromString(A_str);

    if (t[2] != "THEN") return false;

    // -parse  the b or optional c d
    std::vector<WordType> Bs;

    int idx = 3;

    Bs.push_back(typeFromString(t[idx]));
    idx++;

    while (idx < (int)t.size() && t[idx] == "OR") {
        idx++;
        Bs.push_back(typeFromString(t[idx]));
        idx++;
    }

    // find the 'do' key somewhere  (it could be later in the chain)
    if (t[idx] != "DO") return false;
    idx++;

    if (idx >= (int)t.size()) return false;

    std::string action_str = t[idx];

    if (i < 1) return false;

    WordType type_prev  = static_cast<WordType>(sentence_arr[i - 1].type);
    WordType type_curr  = static_cast<WordType>(sentence_arr[i].type);

    std::string word_prev = sentence_arr[i - 1].word;
    std::string word_curr = sentence_arr[i].word;
    
    std::string translation_prev = sentence_arr[i - 1].translation;
    std::string translation_curr = sentence_arr[i].translation;

    // Check first condition: previous == A
    if (type_prev != A) return false;

    // Check second condition: current == one of Bs
    bool matchesB = false;
    for (size_t k = 0; k < Bs.size(); ++k) {
        if (Bs[k] == type_curr) {
            matchesB = true;
            break;
        }
    }

    if (!matchesB) return false;

    // lookup the function you found and call the pointer

    auto func = lookupFunction(action_str.c_str());
    if (func) {
        func(reordered_arr, sentence_arr[i], sentence_arr[i - 1]);
        return true;
    }

    return false;
}



// this is the ugliest code you will see ever, do not pay attention to it :(
// lowk might work tho, i'll tune the parameters later
inline std::string detect_language(const char* sentence) {
    std::string language = "Not Sure.";
    std::vector<std::string> tokens = tokenize(std::string(sentence));

    float en = 0.0f;
    float pt = 0.0f;
    float ja = 0.0f;
    float es = 0.0f;
    float fr = 0.0f;
    float zh = 0.0f;
    float sv = 0.0f;
    float my = 0.0f;

    for (size_t i = 0; i < tokens.size(); i++) {
        const std::string& word = tokens[i];

        
        if(word == "o" || word == "e") {
            pt += 0.5f; 
            en += 0.5f;
        };

        if(word.length() > 1 && std::string(word).substr(word.length() - 2) == "ll"){
                   en += 1.0f;
        }
        if(word == "i") en += 0.5f;
        if(word == "y"){ es += 0.5f; fr += 0.4f;}

        if(word == "em" || word == "sou" || word == "eu"){ pt += 1.0f;}

        if(word == "je" || word == "moi") {fr += 1.0f;}

        if(word == "are" || word == "you") {en += 1.0f;}

        if(word == "lo" || word == "la") {es += 1.0f;}

        if(word == "tu") {fr += 0.5f; pt += 0.5f; es += 0.5f;}
        if(word == "te") {fr += 0.5f; pt += 0.5f; es += 0.5f;}
        if(word.back() == 'g') en += 7.0f;
        auto first_char = [](const std::string &s) -> std::string {
            if (s.empty()) return "";
            size_t len = 1;
            unsigned char c = static_cast<unsigned char>(s[0]);
            if ((c & 0x80) == 0x00) len = 1;    
            else if ((c & 0xE0) == 0xC0) len = 2;
            else if ((c & 0xF0) == 0xE0) len = 3;  
            else if ((c & 0xF8) == 0xF0) len = 4;  
            return s.substr(0, len);
        };

        auto last_char = [](const std::string &s) -> std::string {
            if (s.empty()) return "";
            size_t i = s.size();
            while (i > 0 && (static_cast<unsigned char>(s[i-1]) & 0xC0) == 0x80) --i;
            return s.substr(i-1);
        };

        if (last_char(word) == std::string("é")) { 
            fr += 0.7f;
            pt += 0.4f;
            es += 0.7f;
        }
        if (last_char(word) == std::string("ó")) { 
            pt += 0.4f;
            es += 0.8f;
        }
        if (first_char(word) == std::string("ç")) { 
            pt -= 1.0f;
            fr += 0.6f;
        }
        if (word.find("ñ") != std::string::npos){
            es += 1.0f;
        }
        if (word.find("ç") != std::string::npos){
            pt += 0.7f; fr += 0.6f;
        }
         if (word.find("k") != std::string::npos || word.find("y") != std::string::npos || word.find("w") != std::string::npos){
            pt -= 1.0f;
        }
         if (word.find("å") != std::string::npos || word.find("ä") != std::string::npos || word.find("ö") != std::string::npos){
            sv += 0.9f;
            pt -= 1.0f;
            en -= 1.0f;
            es -= 1.0f;
            fr -= 1.0f;
        }
        
        if (word.find("ão") != std::string::npos)
           { pt += 0.9f;}
        if (word.find("yo") != std::string::npos)
            {es += 0.6f; en += 0.7f; fr -= 0.8f; ja += 0.5;
            }
        if (word.find("nh") != std::string::npos || word.find("lh") != std::string::npos){
            pt += 1.0f;}

        if (word.find("sh") != std::string::npos || word.find("wr") != std::string::npos || word.find("ys") != std::string::npos || word.find("hy") != std::string::npos)
         {   en += 1.0f;}

        if(word.find("wh") != std::string::npos)
         {  en += 1.0f;}
        
           if (word.find("ux") != std::string::npos || word.find("ée") != std::string::npos)
          {  fr += 0.8f;}
           if (word.find("uis") != std::string::npos)
          {  fr += 1.0f;}
            if (word.find("ois") != std::string::npos)
          {  fr += 0.6f;pt += 0.6f;}
         if (word.find("um") != std::string::npos || word.find("tu") != std::string::npos)
          {  pt += 0.7f;}
           if (word.find("mbre") != std::string::npos)
          { 
            en += 0.2f;
             es += 0.7f;
        }


        if (word.find("ph") != std::string::npos) {
            en += 0.4f;
            fr += 0.2f;
        }
        if (word.find("eu") != std::string::npos) {
            fr += 0.3f;
            pt += 0.7f;
        }

        for (size_t j = 0; j < word.size(); ++j) {
            uint32_t unicode_c = next_utf8_char(word, j);
            // does it have hiragana
            if (unicode_c >= 0x3040 && unicode_c <= 0x309F) ja += 1.0;
            // does it have katakana
            if (unicode_c >= 0x30A0 && unicode_c <= 0x30FF) ja += 1.0;
            // does it have ideograms?
            if (unicode_c >= 0x4E00 && unicode_c <= 0x9FFF){
                ja += 0.5;
                zh += 0.5;
            };
                if(unicode_c == 0x6211 || unicode_c == 0x4F60 ){
                   ja -= 1.0;
                   zh += 1.0;     
                }
        }
    }
    
    float maxScore = en;
    if (pt > maxScore) maxScore = pt;
    if (ja > maxScore) maxScore = ja;
    if (es > maxScore) maxScore = es;
    if (fr > maxScore) maxScore = fr;
    if (zh > maxScore) maxScore = zh;
    if (sv > maxScore) maxScore = sv;
    if (my > maxScore) maxScore = my;

    if (maxScore == 0.0f) {
        language = "unknown";
    } else if (maxScore == en) {
        language = "en";
    } else if (maxScore == pt) {
        language = "pt";
    } else if (maxScore == ja) {
        language = "ja";
    } else if (maxScore == es) {
        language = "es";
    } else if (maxScore == fr) {
        language = "fr";
    }else if (maxScore == zh) {
        language = "zh";
    }else if (maxScore == sv) {
        language = "sv";
    }else if (maxScore == my) {
        language = "my";
    }
    return language;
}




inline std::string translate(const char* sentence, const char* from, const char* to, int script = 2){
    // script will be passed to languages that can be written in more than one script
    // japanese (0 = kana, 1 = kana + kanji, 2 = romaji ), malay(0 = rumi, 1 = jawi) and mandarin chinese (0 = hanzi, 1 = pinyin) 
    //  are the ones i plan to implement

    std::string f(from), t(to);
    #if defined(PT_EN) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "en" || t == "EN")) {
            return traduzir_en(sentence);
        }
    #endif
    #if defined(PT_ES) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "es" || t == "ES")) {
            return traduzir_es(sentence);
        }
    #endif
    #if defined(PT_SV) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "sv" || t == "SV")) {
            return traduzir_sv(sentence);
        }
    #endif
    #if defined(EN_JA) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "ja" || t == "JA")) {
            return translate_ja(sentence, script);
        }
    #endif
  #if defined(EN_PT) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "pt" || t == "PT")) {
            return translate_pt(sentence);
        }
    #endif

      return "No translation module found :(";
}


struct Outcome {
    const char* word;
    float score;
    int type;
};

struct Homonym {
    const char* word;
    Outcome* outcomes; 
    size_t num_outcomes;
    const char** tokens;
    const int* forbidden_previous_type;
    size_t num_tokens;
};

extern Homonym homonyms[];
extern const size_t homonymCount;

// this can be global, since specific cases are defined on the structs and not on the function itself
inline std::string semantics(const std::vector<std::string>& sentence,
                             const std::vector<int>& word_types,
                             size_t index,
                             Homonym* homonyms,
                             size_t numHomonyms)
{
    if (index >= sentence.size()) return sentence[index];

    const std::string& w = sentence[index];
   for (size_t h = 0; h < numHomonyms; ++h) {
        Homonym& hom = homonyms[h];
        if (!hom.word) continue;
        if (std::string(hom.word) != w) continue;

        // Reset outcome scores
        for (size_t j = 0; j < hom.num_outcomes; ++j) {
            if (hom.outcomes && j < hom.num_outcomes) {
                hom.outcomes[j].score = 0.0f;
            }
        }

        // Check context words
        int offsets[] = {-2, -1, 1, 2};
        for (int o : offsets) {
            int nearbyIdx = static_cast<int>(index) + o;
            if (nearbyIdx < 0 || nearbyIdx >= static_cast<int>(sentence.size())) continue;

            const std::string& nearby = sentence[nearbyIdx];

            size_t currentOutcome = 0;
            if (!hom.tokens) continue;

            for (size_t tidx = 0; tidx < hom.num_tokens && currentOutcome < hom.num_outcomes; ++tidx) {
                std::string token = hom.tokens[tidx];
     
                if (token == "$") { 
                    ++currentOutcome; 
                    continue; 
                }

                if (nearby == token) {
                    bool forbidden = false;

                    int prevIdx = nearbyIdx - 1; 
                    int prevType = (prevIdx >= 0) ? word_types[prevIdx] : -1;

                    if (prevType >= 0 && hom.forbidden_previous_type) {
                        if (prevType == hom.forbidden_previous_type[currentOutcome]) {
                            forbidden = true;
                        }
                    }

                    if (!forbidden && hom.outcomes && currentOutcome < hom.num_outcomes) {
                        hom.outcomes[currentOutcome].score += 1.0f;
                    }

                    break;
                }

            }
        }

        // Pick best outcome
        float bestScore = -1.0f;
        std::string bestWord = w;
        for (size_t j = 0; j < hom.num_outcomes; ++j) {
            if (hom.outcomes && j < hom.num_outcomes) {
                Outcome& o = hom.outcomes[j];
                if (o.score > bestScore) {
                    bestScore = o.score;
                    bestWord = o.word ? std::string(o.word) : w;
                }
            }
        }
        return bestWord;
    }

    return w;
}



/* ------- SHARED FUNCTIONS ----------------
|           some pairs have to use         |  
------------------------------------------*/

// PT


#if defined(PT_EN) || defined(PT_ES) || defined(ALL)

//is the word visibly not imported from a different language? 
// had to do this since stuff like 'website' is conflicting with verb ending match 'ite',
// but if we infer wether or not the word is an import, we can stop that
inline bool isPortuguese(const std::string& word){

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

inline bool isDiminutive_PT(const std::string& s, const char* suffix) {
    size_t n = 0;
    while (suffix[n] != '\0') n++; 
    if (s.size() < n) return false;
    for (size_t i = 0; i < n; i++) {
        if (s[s.size() - n + i] != suffix[i]) return false;
    }
    return true;
}


#endif



#endif
