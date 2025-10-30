#include "src/lang_it.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 

bool isFile(const std::string& filename) {
    return filename.size() >= 4 && filename.compare(filename.size() - 4, 4, ".txt") == 0;
}

int main(int argc, char* argv[]) {
    bool replMode = (argc == 4 && std::strcmp(argv[1], "REPL") == 0);
    bool fileMode = (argc == 4 && isFile(argv[1]));
    bool newFile = (argc == 5 && isFile(argv[1]) && isFile(argv[4]));

    if (replMode) {
        const char* from = argv[2];
        const char* to = argv[3];

        const char* sample = (std::strcmp(from, "pt") == 0)
            ? "Olá mundo! O tradutor funciona normalmente."
            : (std::strcmp(from, "en") == 0)
            ? "Hello world! The translator works normally."
            : (std::strcmp(from, "es") == 0)
            ? "Hola mundo! El traductor funcciona normalmente."
            : ".";

         const char* prompt = (std::strcmp(from, "pt") == 0)
            ? "O que deseja traduzir?"
            : (std::strcmp(from, "en") == 0)
            ? "What do you want to translate?"
            : (std::strcmp(from, "es") == 0)
            ? "Que deseas traducir?"
            : ".";

         const char* quit_message = (std::strcmp(from, "pt") == 0)
            ? "Digite 'sair' para encerrar.\n"
            : (std::strcmp(from, "en") == 0)
            ? "Type 'exit' to close.\n"
            : (std::strcmp(from, "es") == 0)
            ? "Digite 'salir' para cerrar."
            : ".";

        const char* quit_cmd = (std::strcmp(from, "pt") == 0)
            ? "sair"
            : (std::strcmp(from, "en") == 0)
            ? "exit"
            : (std::strcmp(from, "es") == 0)
            ? "salir"
            : ".";
        
        std::cout << sample << " -> " << translate(sample, from, to, 2) << std::endl;
        std::cout << prompt << " (" << from << "-" << to << ")\n";
        std::cout << quit_message;

        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input == quit_cmd) break;
            std::cout << translate(input.c_str(), from, to, 2) << std::endl;
        }

        return 0;
    }

    if (fileMode) {
        const char* filename = argv[1];
        const char* from = argv[2];
        const char* to = argv[3];

        
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "File: " << filename << " not found." << std::endl;
            return 1;
        }

        std::string line;
        while (std::getline(infile, line)) {
            std::cout << translate(line.c_str(), from, to, 2) << std::endl;
        }

        return 0;
    }

    if (newFile) {
    const char* filename = argv[1];
    const char* from = argv[2];
    const char* to = argv[3];
    const char* destination = argv[4];

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "File: " << filename << " not found." << std::endl;
        return 1;
    }

    std::ofstream outfile(destination);
    if (!outfile) {
        std::cerr << "Could not create file: " << destination << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        outfile << translate(line.c_str(), from, to, 2) << std::endl;
    }

    std::cout << "File translated successfully: " << destination << "from " << from << "to " << to << "." << std::endl;
    return 0;
}



    if (argc < 4) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " <sentence> <from> <to>\n"
                  << "  " << argv[0] << " REPL <from> <to>\n"
                  << "  " << argv[0] << " <file.txt> <from> <to>\n"
                  << "  " << argv[0] << " <file.txt> <from> <to> <file2.txt>\n";
        return 1;
    }

    const char* original_sentence = argv[1];
    const char* from = argv[2];
    const char* to = argv[3];

    std::cout << translate(original_sentence, from, to, 2) << std::endl;
    return 0;
}
