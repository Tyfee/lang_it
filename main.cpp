
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <utility>
#include <sstream> 

using namespace std;

// noun dictionary, not only nouns anymore lol
// basically every word that can't be matched with rules of breakdown will be translated directly from here
map<string, string> nouns = {
  {"melancia", "watermelon"},
  {"areia", "sand"},
  {"chuva", "rain"},
  {"chuva", "rain"},
  {"cachorro", "dog"}
};

map <string, string> art = {
  {"o", "the"},
  {"a", "the"},
  {"do que", "than"}
};

map <string, string> pre = {
  {"do", "of"},
  {"da", "of"},
  {"de", "of"}
};

// nominative/personal pronouns
map<string, string> pro = {
  {"eu", {"i"}},
  {"você", {"you"}},
  {"nós",  {"we"}},
  {"ele",  {"he"}},
  {"ela",  {"she"}},
  {"elas",  {"they"}},
  {"eles",  {"they"}}
};

//object pronoun match (in english)

map<string, string> obj_pro = {
  {"she", {"her"}},
  {"he", {"him"}},
  {"they", {"them"}}
};

// oblique pronouns
map<string, string> obl_pro = {
  {"te", "you"},
  {"me", "me"},
};

// adjectives
map<string, string> adj = {
  {"azul", "blue"},
  {"bonito", "beautiful"},
  {"legal", "cool"},
  {"grande", "big"},
  {"forte", "strong"},
  {"mais", "more"}
};

//adverbs

map<string, string> adv = {
  {"mas", "but"},
  {"quando", "when"},
  {"quem", "who"}
};

// verb prefixes where 0 = regular, 1 = irregular conjugation
map<string, pair<string, int>> verbs = {
  {"am", {"lov", 0}},
  {"gost", {"lik", 0}},
  {"pens", {"think", 1}},
  {"quer", {"want", 0}},
  {"corr", {"run", 1}},
  {"abr", {"open", 0}},
  {"fech", {"clos", 0}}
};

// common suffixes with traceable trnaslation pattern
map<string, string> suff = {
  {"mente", "ly"},
  {"ável", "able"},
  {"ível", "ible"},
  {"ória", "ory"},
  {"ência", "ency"},
  {"cidade", "city"}
};

// dictionary lookup

//look for preffix matches 
pair<string, int> prefixLookup(string word){
  string translation = word; 
  int word_type;

  vector<string> infinitive = {"ar", "er", "ir"};
  vector<string> present_non_s = {"o", "to", "go", "ro", "am", "em", "mos"};
  vector<string> present_s = {"a", "ta", "re", "ga"};
  vector<string> general_past = {"ei", "ou", "eu", "ti", "aram", "ia"};

  string reg_past_ending = "ed";
  string inf_ending = "e";
  string inf_ending_third = "es";

  // this will try to find a verb ending that can be translated to past tense or infinitive or continuous or whatever
  // it's a lambda that returns a pair with the match lemma + the vowel/conjugation.
auto find_verb = [](vector<string> format, string word, string ending) {
  for(size_t i = 0; i < format.size(); ++i){
     string translation_;
     int word_type_;
     int verb_type;
     
      size_t match = word.rfind(format[i]);
      if (match != string::npos && match + format[i].length() == word.length()) {
       string root = word.substr(0, match);
       
       if(verbs.find(root) != verbs.end()) {
           translation_ = verbs[root].first + ending;
           word_type_ = 3;
           verb_type = verbs[root].second;
           cout << verb_type;
           return pair<string, int>{translation_, word_type_};
       }
     }
  }
      return pair<string, int>{"", -1};
};
// try with every possible ending set, infinitive being the first match attempt
auto result_set = find_verb(infinitive, word, inf_ending);

if (result_set.first.empty()) 
    result_set = find_verb(general_past, word, reg_past_ending);

if (result_set.first.empty()) 
    result_set = find_verb(present_non_s, word, inf_ending);

if (result_set.first.empty()) 
    result_set = find_verb(present_s, word, inf_ending_third);

  return result_set;
}

//look for suffix matches
pair<string, int> suffixLookup(string word){
  
 string translation;
 int word_type;
 // TODO, make this smaller and not manually check if

 // if the last 6 letters of a word match a suff. e.g: totalmente, strip the 6 letters and look up
 // the corresposing translation for the length-6 suffix
 if(word.length() > 6){
  if(suff.count(word.substr(6))){
    translation = word.substr(0, 6) + suff[word.substr(6)];
    
  word_type = 2;
  }
  //same shit 
  else if(suff.count(word.substr(5))){
    translation = word.substr(0, 5) + suff[word.substr(5)];
  }
  //same shit 
  else if(suff.count(word.substr(4))){
    translation = word.substr(0, 4) + suff[word.substr(4)];
  }
  else if(suff.count(word.substr(3))){
    translation = word.substr(0, 3) + suff[word.substr(3)];
  }
  else{
    translation = "$";
    
  }
}
  return {translation, word_type};
}

pair<string, int> nounLookup(string word){
  // TODO: Creaate hierarchy for word category
  string translation;
  // 0 = noun 1 = adj 2 = adverb 3 = verb 4 = pronoun
  int word_type;
  
  // rules
   bool plural = nouns.count(word.substr(0, word.length() - 1));
   bool gender_shift = nouns.count(word.substr(0, word.length() - 1)  + "o");

  // for each individual word loop, you look in the noun dictionary
   if(nouns.count(word)){
   translation = nouns[word];
   word_type = 0;
   }
   else if(adj.count(word)){
      translation = adj[word];
      word_type = 1;
    }
    else if(pro.count(word)){
      translation = pro[word];
      word_type = 4;
    }
    
    else if(obl_pro.count(word)){
      translation = obl_pro[word];
      word_type = 11;
    }

     else if(art.count(word)){
      translation = art[word];
      word_type = 9;
    }
     else if(pre.count(word)){
      translation = pre[word];
      word_type = 8;
    }
    
     else if(adv.count(word)){
      translation = adv[word];
      word_type = 13;
    }
   else if(plural){
     
    // by removing the last letter of the word, we can check for **BASIC** plural. e.g casa[s] -> casa
         translation = nouns[word.substr(0, word.length() - 1)] + "s";
         
    } // by switching the last letter of the word, we can check for **BASIC** gender shift. e.g cachorra -> cachorro - a + o -> cachorro  
   else if(gender_shift){
          
         translation = nouns[word.substr(0, word.length() - 1) + "o"];
        }
       // if not found suffix match
        else if(suffixLookup(word).first.length() > 0){ 
         translation = suffixLookup(word).first;
       }else if(prefixLookup(word).first.length() > 0){
        // if suffix not found, look for prefix
        translation = prefixLookup(word).first;
        word_type = 3;
       }
   
    cout << translation + " " << word_type << " ";
  return {translation, word_type};
}

// when words need to switch order
// this is actually various manipulations (Take Off The Blindfold REFERENCE????)
//not only word order, but i'm not changing the name at this point
vector<pair<string, int>> reorder_helpers(vector<pair<string, int>> sentence_arr) {
    vector<pair<string, int>> reordered_arr;

    for (size_t i = 0; i < sentence_arr.size(); ++i) {
    // ------------------------ ADJECTIVE ORDER  -----------------
    // a set is noun[0] and adjective[1], we switch order, so that casa[0] azul[1] -> blue[1] house[0]
      if (i > 0 && sentence_arr[i - 1].second == 0 && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 1]);
        } 
       // ------------------------ OBLIQUE PRONOUNS  -----------------
       // a set is oblique pronoun[11] and verb[3], we switch order, so that te[11] amo[3] -> love[3] you[11]
        else if (i > 0 && sentence_arr[i - 1].second == 11 && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i]);  
            reordered_arr.push_back(sentence_arr[i - 1]);
        } 

          // ------------------------ SUPERLATIVE  --------------------
        // a set is word1 = 'o/a' and word2 = 'mais' and word3 = adj[1], we eliminate the 'mais', so that 'o mais forte[2]' -> 'the strongest'
        else if (i > 1 && sentence_arr[i - 2].second == 9  && sentence_arr[i - 1].first == "more" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i].first + "est", 20});  
        } 
        // ------------------------ COMPARATIVE  ----------------- TODO: exceptions such as beautiful[er], good[er], badd[er], might be better as bigrams
        // a set is word1 = 'mais' and word2 = adj[1], we eliminate the first 'mais' and append 'er' to word2, so that mais forte[2] -> stronger
        else if (i > 0 && sentence_arr[i - 1].first == "more" && sentence_arr[i].second == 1) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(pair<string, int>{sentence_arr[i].first + "er", 20});  
        } 
      // ------------------------ INTRANSITIVE VERBS THAT TAKE 'DE'  ----------------- 
        // a set is word1 = verb[3] and word2 = 'de', we eliminate the preposition 'de', so that gosto[1] de  -> stronger
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].first == "of") {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
        } 
  // ------------------------ TRANSITIVE VERBS WITH A PERSONAL PRONOUN  ----------------- 
        // a set is word1 = verb[3] and word2 = pronoun[4], we use the second value of the pair? of the pronoun i guess idk i'm tired
        // like the value of the key 'ela' is a pair<string, string> that holds both the subject and object pronoun {'she', 'her'}
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].second == 4) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);
            reordered_arr.push_back(pair<string, int>{obj_pro[sentence_arr[i].first], 10});  
        } 


    // ------------------------ DOUBLE VERBS ----------------- TODO: figure out if verb1 verb[2]-ing is needed at some case
    // a set is verb[3] and verb[3], we add 'to' between them, so that amo[3] correr[3] -> love[3] *to* run[3]
        else if (i > 0 && sentence_arr[i - 1].second == 3 && sentence_arr[i].second == 3) {
            reordered_arr.pop_back(); 
            reordered_arr.push_back(sentence_arr[i - 1]);  
            reordered_arr.push_back(pair<string, int>{"to", -1});  
            reordered_arr.push_back(sentence_arr[i]);
        } 
        
        else {
            reordered_arr.push_back(sentence_arr[i]);
        }
    }

    return reordered_arr;
}

//ngram groups
std::string unigramLookup(vector<string> array_of_words){

  vector<pair<string, int>> sentence_arr;

  string sentence;
  for(size_t i = 0; i < array_of_words.size(); ++i){
    pair<string, int> match = nounLookup(array_of_words[i]);
    sentence_arr.push_back({match.first, match.second});
  }
  sentence_arr = reorder_helpers(sentence_arr);
  for(size_t i = 0; i < sentence_arr.size(); ++i){
     sentence = sentence + " " + sentence_arr[i].first;
  }
  cout << "\n" << sentence << "\n";
  return sentence;
}

std::string bigramLookup(vector<string> array_of_words){
  vector<string> bi_array_of_words = array_of_words;
  
  return unigramLookup(bi_array_of_words);
}

std::string trigramLookup(vector<string> array_of_words){
    vector<string> tri_array_of_words = array_of_words;
  
    return bigramLookup(tri_array_of_words);
}

vector<string> traduzir(string sentence) {
    vector<string> arr;
    istringstream iss(sentence);
    string word;
    
    while (iss >> word) {
        arr.push_back(word);
    }

    for(size_t i = 0; i < arr.size(); ++i){
      // cout << arr[i] << ", ";
    }
   // cout << "\n";
    trigramLookup(arr);
    return arr;
}
//main body
  int main (){
   
    string original_sentence;

    std::cout << "O que deseja traduzir (pt-en)?\n";
    getline(cin, original_sentence);
    traduzir(original_sentence + " ");
    return 0;
  
}
