#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    char original_sentence[250];
    const char* from = "pt";
    const char* to = "es";
    const char* sample = (from == "pt" ? "Olá mundo! O tradutor funciona normalmente." : from == "en" ? "Hello world! The translator works normally" : ".");
    std::cout << sample << " -> ";
    std::cout << translate(sample, from, to) << std::endl;
    std::cout << "O que deseja traduzir? (" << from << "-" << to << ")\n";
    std::cout << "Digite 'sair' para encerrar.\n";

    while (true) {
        std::cin.getline(original_sentence, 250);
        if (original_sentence == "sair") break;
        std::cout << translate(original_sentence, from, to) << std::endl;
    }

    return 0;
}
