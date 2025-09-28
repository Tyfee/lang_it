#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>
#include <sstream> 
#include "../lang_it.h"

using namespace std;

typedef struct
{
  const char* w;
  const char* t;
} Entry;

constexpr Entry dict[] = {
      {"i", "私"}
};
constexpr Entry fixed_ngrams[] = {
  {"thank_you", "ありがとう"}
};


template <size_t N>
static const char* lookup(const Entry (&dict)[N], const char* word) {
    for (size_t i = 0; i < N; ++i) {
        const char* p = dict[i].w;
        const char* q = word;
        while (*p && *q && *p == *q) { ++p; ++q; }
        if (*p == *q) return dict[i].t;
    }
    return nullptr;
}


static void to_lower(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] - 'A' + 'a';
        }
    }
}
static vector<string> tokenize(const string &text) {
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


enum Kana {
    // Hiragana
    A = 1, KA, SA, TA, NA, HA, MA, YA, RA, WA, N,
    I, KI, SHI, CHI, NI, HI, MI, RI,
    U, KU, SU, TSU, NU, HU, MU, YU, RU
};

const string HIRAGANA[] = {
    "",    
    "あ", "か", "さ", "た","な","は","ま","や","ら","わ","ん",  
    "い","き", "し","ち","に","ひ","み","り",
    "う","く","す","つ","ぬ","ふ","む","ゆ","る" 
};

string turnToKana(int syllable){

    string k = HIRAGANA[syllable];
    return k;
}


static pair<string, int> nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type;

if(lookup(dict, word.c_str())){
   translation = lookup(dict, word.c_str());
   word_type = 0;
   }else{
     translation = word;
     word_type = -1;
   }
   return pair<string, int>{translation, word_type};

}

//ngram groups
static string unigramLookup(vector<string> array_of_words, vector<int> ignore_flags){

  vector<pair<string, int>> sentence_arr;

  string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    pair<string, int> match = nounLookup(array_of_words[i]);
    sentence_arr.push_back({match.first, match.second});
  }

 for (size_t i = 0; i < sentence_arr.size(); ++i) {
    const std::string& token = sentence_arr[i].first;

    char firstChar = token.empty() ? '\0' : token[0];
    bool isPunctuation = (firstChar == '?' || firstChar == '!' || 
                          firstChar == '.' || firstChar == ','
                          || firstChar == '-');

    if (!sentence.empty() && !isPunctuation) {
        sentence += " ";
    }

    sentence += token;
}
  return sentence;
}

static string bigramLookup(vector<string> array_of_words){
  vector<string> bi_array_of_words = array_of_words;
  // 0 is false lolz
  int should_ignore = 0;
  
  // if the sentence has 2 or more words, we can look up word bigrams
vector<string> mended_array_of_words;
vector<int> ignore_flags;

  size_t i = 1;
  while (i < bi_array_of_words.size()) {
      string bigram = bi_array_of_words[i-1] + "_" + bi_array_of_words[i];
      if (lookup(fixed_ngrams, bigram.c_str())) {
          mended_array_of_words.push_back(lookup(fixed_ngrams, bigram.c_str()));
          ignore_flags.push_back(1);  // mark this word as "already translated"
          i += 2;
      } else {
          mended_array_of_words.push_back(bi_array_of_words[i-1]);
          ignore_flags.push_back(0);  // normal word
          i += 1; 
      }
  }
  if (i == bi_array_of_words.size()) {
      mended_array_of_words.push_back(bi_array_of_words.back());
      ignore_flags.push_back(0);
  }

  return unigramLookup(mended_array_of_words, ignore_flags);
    }


static string trigramLookup(vector<string> array_of_words){
    vector<string> tri_array_of_words = array_of_words;
  
    return bigramLookup(tri_array_of_words);
}

string translate_ja(string sentence) {
    
    to_lower(&sentence[0]); 
    vector<string> arr = tokenize(sentence);  
    string translated = trigramLookup(arr);
    return translated; 

}