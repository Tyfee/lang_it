#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    std::string original_sentence;
    std::cout << "Olá mundo! O tradutor funciona normalmente. -> ";
    std::cout << traduzir_en("Olá mundo! O tradutor funciona normalmente.") << std::endl;
    std::cout << "Para mais informações, visite o github: https://github.com/Tyfee/lang_it -> ";
     std::cout << traduzir_en("Para mais detalhes, visite o github: https://github.com/Tyfee/lang_it") << std::endl;
    std::cout << "O que deseja traduzir (pt-en)?\n";
    std::cout << "Digite 'sair' para encerrar.\n";

    while (true) {
        std::getline(std::cin, original_sentence);
        if (original_sentence == "sair") break;
        std::cout << traduzir_en(original_sentence) << std::endl;
    }

    return 0;
}
