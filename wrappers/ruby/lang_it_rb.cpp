#include <rice/rice.hpp>
#include <rice/stl.hpp>
#include <string>

#include "../../lang_it.h" 

using namespace Rice;

std::string rb_detect_language(const char* sentence) {
    return detect_language(sentence);
}

std::string rb_translate(const char* sentence, const char* from, const char* to) {
    return translate(sentence, from, to);
}

extern "C"
void Init_lang_it_rb() {
    Module rb_mLangIt = define_module("LangIt");

    rb_mLangIt
        .define_singleton_function("detect_language", &rb_detect_language)
        .define_singleton_function("translate", &rb_translate);
}
