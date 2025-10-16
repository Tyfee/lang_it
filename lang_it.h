#ifndef LANG_IT_H
#define LANG_IT_H
#include <string>
#include <vector>

// pairs

#if defined(PT_EN) || defined(ALL)
std::string traduzir_en(const char* sentence);
#endif

#if defined(PT_ES) || defined(ALL)
std::string traduzir_es(const char* sentence);
#endif

#if defined(EN_JA) || defined(ALL)
std::string translate_ja(const char* sentence);
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

typedef struct {
    const char* w;
    const char* t;
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




inline std::string detect_language(const char* sentence){
    std::string language = "en";
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
            return translate_ja(sentence);
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






  










/* ------- OPTIONAL FUNCTIONS -------------
|             all pairs CAN use             |  
------------------------------------------*/
typedef struct{
   std::string word;
   std::string most_similar;
   float percentage;
} L_D;

inline L_D levenshtein_distance(std::string word){
    L_D most_similar = {word, word, 0.0};
    return most_similar;
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
