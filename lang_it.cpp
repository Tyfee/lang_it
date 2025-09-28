#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    std::string original_sentence;
    std::cout << "Olá mundo! O tradutor funciona normalmente. -> ";
    std::cout << traduzir_en("Olá mundo! O tradutor funciona normalmente.") << std::endl;
    std::cout << "O que deseja traduzir (pt-en)?\n";
    std::cout << "Digite 'sair' para encerrar.\n";

    while (true) {
        std::getline(std::cin, original_sentence);
        if (original_sentence == "sair") break;
        std::cout << translate_ja(original_sentence) << std::endl;
    }

    return 0;
}
