#include <jni.h>
#include <string>
#include "lang_it.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_tyfee_lang_1it_LangIt_detectLanguage(JNIEnv* env, jclass clazz, jstring sentence) {
    const char* c_sentence = env->GetStringUTFChars(sentence, nullptr);
    std::string result = detect_language(c_sentence);
    env->ReleaseStringUTFChars(sentence, c_sentence);
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jstring JNICALL
Java_com_tyfee_lang_1it_LangIt_translate(JNIEnv* env, jclass clazz, jstring sentence,
                                         jstring from, jstring to, jint script) {
    const char* c_sentence = env->GetStringUTFChars(sentence, nullptr);
    const char* c_from = env->GetStringUTFChars(from, nullptr);
    const char* c_to = env->GetStringUTFChars(to, nullptr);

    std::string result = translate(c_sentence, c_from, c_to,script);

    env->ReleaseStringUTFChars(sentence, c_sentence);
    env->ReleaseStringUTFChars(from, c_from);
    env->ReleaseStringUTFChars(to, c_to);

    return env->NewStringUTF(result.c_str());
}


}
