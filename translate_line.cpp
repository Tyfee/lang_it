#include "lang_it.h"
#include <iostream>
#include <string>

int main() {
    std::string line;
    size_t line_index = 0;

    while (std::getline(std::cin, line)) {
        ++line_index;

        // Skip completely empty lines
        if (line.empty()) {
            std::cout << std::endl; 
            continue;
        }

        try {
            std::string translation = traduzir_en(line);
            std::cout << translation << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error on line " << line_index << ": " << e.what() << std::endl;
            std::cout << std::endl; 
        }
        if (line_index % 100 == 0) {
            std::cerr << "Processed " << line_index << " lines..." << std::endl;
        }
    }

    return 0;
}
