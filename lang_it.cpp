#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    char original_sentence[250];
    std::cout << "Olá mundo! O tradutor funciona normalmente. -> ";
    std::cout << translate_ja("Olá mundo! O tradutor funciona normalmente.") << std::endl;
    std::cout << "O que deseja traduzir (pt-en)?\n";
    std::cout << "Digite 'sair' para encerrar.\n";

    while (true) {
        std::cin.getline(original_sentence, 250);
        if (original_sentence == "sair") break;
        std::cout << translate_ja(original_sentence) << std::endl;
    }

    return 0;
}
