#include "lang_it.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 

bool isFile(const std::string& filename) {
    return filename.size() >= 4 && filename.compare(filename.size() - 4, 4, ".txt") == 0;
}

bool isNumber(const char* s) {
    if (!s || *s == '\0') return false;

    if (*s == '\0') return false;

    while (*s) {
        if (*s < '0' || *s > '9')
            return false;
        s++;
    }
    return true;
}


int main(int argc, char* argv[]) {
    
    bool help = (argc == 2 && std::strcmp(argv[1], "-help") == 0);
    bool info = (argc == 2 && std::strcmp(argv[1], "-info") == 0);
    bool version = (argc == 2 && std::strcmp(argv[1], "-v") == 0);

    bool d_l = (argc == 3 && std::strcmp(argv[1], "-detect") == 0);
    bool l_m = (argc == 3 && std::strcmp(argv[1], "-load") == 0);
    
    bool replMode = std::strcmp(argv[1], "-REPL") == 0 && (argc == 4 ||(argc == 5 && isNumber(argv[4])));

    bool fileMode = (argc == 4 && isFile(argv[1]));
    bool newFile = (argc == 5 && isFile(argv[1]) && isFile(argv[4]));

    if (replMode) {
        const char* from = argv[2];
        const char* to = argv[3];
      int script = 2;

        if (argc == 5 && argv[4]) {
            script = 0;
            for (const char* p = argv[4]; *p; ++p) {
                script = script * 10 + (*p - '0');
            }
        }
        const char* sample = (std::strcmp(from, "pt") == 0)
            ? "Olá mundo! O tradutor funciona normalmente."
            : (std::strcmp(from, "en") == 0)
            ? "Hello world! The translator works normally."
            : (std::strcmp(from, "es") == 0)
            ? "Hola mundo! El traductor funcciona normalmente."
            : (std::strcmp(from, "zh") == 0)
            ? "你好世界！翻译器工作正常"
            : ".";

         const char* prompt = (std::strcmp(from, "pt") == 0)
            ? "O que deseja traduzir?"
            : (std::strcmp(from, "en") == 0)
            ? "What do you want to translate?"
            : (std::strcmp(from, "es") == 0)
            ? "Que deseas traducir?"
            : (std::strcmp(from, "zh") == 0)
            ? "你想翻译什么？"
            : ".";

         const char* quit_message = (std::strcmp(from, "pt") == 0)
            ? "Digite 'sair' para encerrar.\n"
            : (std::strcmp(from, "en") == 0)
            ? "Type 'exit' to close.\n"
            : (std::strcmp(from, "es") == 0)
            ? "Digite 'salir' para cerrar."
            : (std::strcmp(from, "zh") == 0)
            ? "输入“exit”离开\n"
            : ".";

        const char* quit_cmd = (std::strcmp(from, "pt") == 0)
            ? "sair"
            : (std::strcmp(from, "en") == 0)
            ? "exit"
            : (std::strcmp(from, "zh") == 0)
            ? "exit"
            : (std::strcmp(from, "es") == 0)
            ? "salir"
            : ".";
        
        std::cout << sample << " -> " << translate(sample, from, to, script) << std::endl;
        std::cout << prompt << " (" << from << "-" << to << ")\n";
        std::cout << quit_message;

        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input == quit_cmd) break;
            std::cout << translate(input.c_str(), from, to, script) << std::endl;
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
        if (info) {
            #if defined(ALL)

            std::cout << "Available languages: English, Portuguese(BR), Spanish, French, Japanese, Svenska, Maxakali \nBinary size: 707kb\n";
            return 0;
            #endif

        }
        else if (help) {
            std::cout << "Usage:\n\n"
                << "Installed version:" << "lang_it -v\n\n"
                << "Auto detect language:" << "lang_it -detect <sentence>\n\n"
                << "Translate a sentence: " << "lang_it <sentence> <from> <to>\n\n"
                << "Interactive translator:" << "lang_it -REPL <from> <to>\n\n"
                << "Translate files:" << "lang_it <file.txt> <from> <to>\n\n"
                << "Translate a file into another:" << "lang_it <file.txt> <from> <to> <file2.txt>\n\n";

            return 0;
        }
        else if (version) {

            std::cout << "lang_it v0.01";
            return 0;
        }else if(d_l) {

            const char* sentence = argv[2];
            std::cout << detect_language(sentence) << std::endl;
            return 0; 
        }
        else if(l_m) {
            const unsigned char magic_number[] = {0x6C, 0x61, 0x6E, 0x67, 0x00};
            const unsigned char from[] = {0xF0 /* from */, 0x02 /* 2 bytes */, 0x65 , 0x6E /* EN */, 0x00};
            const unsigned char to[] = {0xF1 /* from */, 0x03 /* 2 bytes */, 0x6D , 0x62, 0x6C /* MBL */, 0x00};

            const unsigned char dict_sect[] = {0xD1 /* dict */, 0x01 /* number of entries */,  0x00};

            const unsigned char word_sect[] = {0xF0 /* word */, 0x03 /* length  (3)*/, 0x01 /* number_of_words (1) */, 0x00};
            const unsigned char word[] = { 0x6B, 0x69, 0x64, 0x00};

            const unsigned char translation_sect[] = {0xF1 /* translation */, 0x06 /* length  (3)*/, 0x00};
            const unsigned char translation[] = {0x6B, 0x61, 0x6B, 0x78, 0x6F, 0x70, 0x00};

            const unsigned char type[] = {0xF2 /*type*/, 0x00 /* (0 = noun )*/};



            const unsigned char end[] = {0x00, 0x00, 0x00};
            const unsigned char* full_buffer[] = {magic_number, from, to, dict_sect, end};

            load_from_bin(reinterpret_cast<const char*>(full_buffer));

            const char* sample = "Olá mundo! O tradutor funciona normalmente.";
            const char* prompt = "O que deseja traduzir?";
            const char* quit_message = "Digite 'sair' para encerrar.\n";
            const char* quit_cmd = "sair";
        
        std::cout << sample << " -> " << translate_from_bin(sample) << std::endl;
        std::cout << quit_message;

        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input == quit_cmd) break;
            std::cout << translate_from_bin(input.c_str()) << std::endl;
        }

        return 0;
        }
        else {
            std::cerr << "Usage:\n"
                << "Installed version:" << argv[0] << " -v>\n"
                << "Auto detect language:" << argv[0] << " -detect <sentence>\n"
                << "Load translation module:" << argv[0] << " -load <file.lang>\n"
                << "Translate a sentence: " << argv[0] << " <sentence> <from> <to>\n"
                << "Interactive translator:" << argv[0] << " -REPL <from> <to>\n"
                << "Translate files:" << argv[0] << " <file.txt> <from> <to>\n"
                << "Translate a file into another:" << argv[0] << " <file.txt> <from> <to> <file2.txt>\n";
            return 1;
        }
    }

    const char* original_sentence = argv[1];
    const char* from = argv[2];
    const char* to = argv[3];
      int script = 2;

        if (argc == 5 && argv[4]) {
            script = 0;
            for (const char* p = argv[4]; *p; ++p) {
                script = script * 10 + (*p - '0');
            }
        }
    

    std::cout << translate(original_sentence, from, to, script) << std::endl;
    return 0;
}
