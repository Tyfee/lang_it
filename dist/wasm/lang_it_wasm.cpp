#include "../../src/lang_it.h"
#include <emscripten/bind.h>
#include <string>

std::string translate_it(std::string sentence,
                              std::string from,
                              std::string to, int type) {
    return translate(sentence.c_str(), from.c_str(), to.c_str(), type);
}

std::string detect_it(std::string sentence) {
    return detect_language(sentence.c_str());
}

EMSCRIPTEN_BINDINGS(translator) {
    emscripten::function("translate", &translate_it);
    emscripten::function("detect_language", &detect_it);
}
