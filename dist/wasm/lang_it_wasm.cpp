#include "../../src/lang_it.h"
#include <emscripten/bind.h>
#include <string>

std::string translate_it(std::string sentence,
                              std::string from,
                              std::string to, int type = 0) {
    return translate(sentence.c_str(), from.c_str(), to.c_str(), type);
}

std::string detect_it(std::string sentence) {
    return detect_language(sentence.c_str());
}

std::string translate_from_binary(std::string sentence,
                                  std::string file_buffer)
{
    load_from_bin(
        reinterpret_cast<const uint8_t*>(file_buffer.data()),
        file_buffer.size()
    );

    return translate_from_bin(sentence.c_str());
}


EMSCRIPTEN_BINDINGS(translator) {
    emscripten::function("translate", &translate_it);
    emscripten::function("detect_language", &detect_it);
    emscripten::function("translate_from_bin", &translate_from_binary);
}
