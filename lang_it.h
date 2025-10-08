#ifndef LANG_IT_H
#define LANG_IT_H
#include <string>


// pairs

#ifdef PT_EN
std::string traduzir_en(const char* sentence);
#endif

#ifdef EN_JA
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
    #ifdef PT_EN
        if ((f == "pt" || f == "PT") && (t == "en" || t == "EN")) {
            return traduzir_en(sentence);
        }
    #endif
    #ifdef EN_JA
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
