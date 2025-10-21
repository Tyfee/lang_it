#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    char original_sentence[250];
    std::cout << "Olá mundo! O tradutor funciona normalmente. -> ";
    std::cout << traduzir_en("Olá mundo! O tradutor funciona normalmente.") << std::endl;
    std::cout << "O que deseja traduzir (pt-en)?\n";
    std::cout << "Digite 'sair' para encerrar.\n";

    while (true) {
        std::cin.getline(original_sentence, 250);
        if (original_sentence == "sair") break;
        std::cout << detect_language(original_sentence) << std::endl;
    }

    return 0;
}
