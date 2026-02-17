#include <iostream>
#include <fstream>
#include <string>

#include "lang_it.h"

int main()
{
    // 1️⃣ Open binary dictionary
    std::ifstream file("test.lang", std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cout << "We couldn't open the file.\n";
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0)
    {
        std::cout << "Empty or invalid file.\n";
        return 1;
    }

    // 2️⃣ Allocate buffer
    char* full_buffer = new char[size];

    if (!file.read(full_buffer, size))
    {
        std::cout << "Failed to read file.\n";
        delete[] full_buffer;
        return 1;
    }

    // 3️⃣ Load dictionary from binary
    load_from_bin(full_buffer, size);

    std::cout << "Dictionary loaded.\n";

    // 4️⃣ Test translation
    std::string input;
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, input);

    std::string result = translate_from_bin(input.c_str());

    std::cout << "Translated: " << result << std::endl;

    // ⚠️ DO NOT delete buffer if your dictionary stores raw pointers into it
    // delete[] full_buffer;

    return 0;
}
