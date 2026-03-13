#include "../../src/lang_it.h"
#include <emscripten/bind.h>
#include <string>

std::string translate_it(std::string sentence,
                         std::string from,
                         std::string to, 
                         int type = 0) {
    return translate(sentence.c_str(), from.c_str(), to.c_str(), type);
}

#ifdef ALLOW_IMPORTS

static bool binary_loaded = false;

void load_binary(std::string file_buffer) {
    load_from_bin(
        reinterpret_cast<const uint8_t*>(file_buffer.data()),
        file_buffer.size()
    );
    binary_loaded = true;
}

std::string translate_loaded(std::string sentence) {
    return translate_from_bin(sentence.c_str());
}

bool is_loaded() {
    return binary_loaded;
}

#endif

EMSCRIPTEN_BINDINGS(translator) {
    emscripten::function("translate", &translate_it);
    
    #ifdef ALLOW_IMPORTS
    emscripten::function("load_binary", &load_binary);
    emscripten::function("translate_loaded", &translate_loaded);
    emscripten::function("is_loaded", &is_loaded);
    #endif
}