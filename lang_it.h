#ifndef LANG_IT_H
#define LANG_IT_H
#include <string>


// pairs

#if defined(PT_EN) || defined(ALL)
std::string traduzir_en(const char* sentence);
#endif

#if defined(EN_JA) || defined(ALL)
std::string translate_ja(const char* sentence);
#endif

/* ------- GLOBAL CORE FUNCTIONS -----------
|                                          |  
------------------------------------------*/


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
    #if defined(EN_JA) || defined(ALL)
        if ((f == "en" || f == "EN") && (t == "ja" || t == "JA")) {
            return translate_ja(sentence);
        }
    #endif

      return "No translation module found :(";
}


/* ------- OPTIONAL FUNCTIONS -------------
|                                          |  
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

#endif
