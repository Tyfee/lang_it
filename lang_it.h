#ifndef LANG_IT_H
#define LANG_IT_H
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
std::string translate_ja(const char* sentence, bool kanji);
#endif



















/* ------- GLOBAL CORE FUNCTIONS -----------
|           all pairs use                  |  
------------------------------------------*/



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

enum Flags: uint8_t {
    ANIMATE = 0,
    IS_HUMAN = 1 << 0,
    NO_PLURAL = 1 << 1,
    IRREGULAR_PLURAL = 1 << 2,
    IS_PLACE = 1 << 3,
    IN_ON = 1 << 4,
    UNCOUNTABLE = 1 << 5
};

typedef struct {
    const char* w;
    const char* t;
    uint8_t flags;
} Entry;

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

    for (size_t i = 0; i < tokens.size(); i++) {
        const std::string& word = tokens[i];

        
        if(word == "o" || word == "e") {
            pt += 0.5f; 
            en += 0.5f;
        };

        if(word.length() > 1 && std::string(word).substr(word.length() - 2) == "ll"){
                   en += 1.0;
        }
        if(word == "i") en += 0.5f;
        if(word == "y"){ es += 0.5f; fr += 0.4;}

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
            fr += 0.7;
            pt += 0.4;
            es += 0.7;
        }
        if (last_char(word) == std::string("ó")) { 
            pt += 0.4;
            es += 0.8;
        }
        if (first_char(word) == std::string("ç")) { 
            pt -= 1.0;
            fr += 0.6;
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
        
        if (word.find("ão") != std::string::npos)
           { pt += 0.9f;}
        if (word.find("yo") != std::string::npos)
            {es += 0.6f; en += 0.7f; fr -= 0.8f;
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
            if (unicode_c >= 0x4E00 && unicode_c <= 0x9FFF) ja += 0.5;
        }
    }
    
    float maxScore = en;
    if (pt > maxScore) maxScore = pt;
    if (ja > maxScore) maxScore = ja;
    if (es > maxScore) maxScore = es;
    if (fr > maxScore) maxScore = fr;

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
    }
    return language;
}



inline std::string translate(const char* sentence, const char* from, const char* to){
    std::string f(from), t(to);
    #if defined(PT_EN) || defined(ALL)
        if ((f == "pt" || f == "PT") && (t == "en" || t == "EN")) {
            return traduzir_en(sentence);
        }
    #endif
    #if defined(PT_ES) || defined(ALL)
        if ((f == "pt" || f == "ES") && (t == "es" || t == "ES")) {
            return traduzir_es(sentence);
        }
    #endif
    #if defined(EN_JA) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "ja" || t == "JA")) {
            return translate_ja(sentence, true);
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
    size_t num_tokens;
};

extern Homonym homonyms[];
extern const size_t homonymCount;

// this can be global, since specific cases are defined on the structs and not on the function itself
inline std::string semantics(const std::vector<std::string>& sentence,
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
                    if (hom.outcomes && currentOutcome < hom.num_outcomes) {
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
#endif



#endif
