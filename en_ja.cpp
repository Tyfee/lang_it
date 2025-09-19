#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <utility>
#include <algorithm>
#include <sstream> 
#include "lang_it.h"

  int main (){
   
    string original_sentence;

    std::cout << "What do you want to translate (en-ja)?\n";
    cout << "Type 'exit' to close.\n";
    while(true){
    getline(cin, original_sentence);
    translate_ja(original_sentence + " ");
    
    if (original_sentence == "exit")
      break;
    }

    return 0;
  
}