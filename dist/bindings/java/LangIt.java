package com.example.myapp;

import java.text.Normalizer;

public class LangIt {
    static {
        System.loadLibrary("lang_it_jni"); 
    }

    public native static String detectLanguage(String sentence);
    public native static String translate(String sentence, String from, String to);

    public static String normalize(String input) {
        return Normalizer.normalize(input, Normalizer.Form.NFD)
                         .replaceAll("\\p{InCombiningDiacriticalMarks}+", "");
    }
}
