cmake_minimum_required(VERSION 3.22.1)
project("lang_it_jni")

add_library(lang_it_jni SHARED
        lang_it_jni.cpp
        pairs/en_ja.cpp
        pairs/en_pt.cpp
        pairs/es_fr.cpp
        pairs/fr_pt.cpp
        pairs/pt_en.cpp
        pairs/pt_es.cpp
)

include_directories(
        .
        pairs
)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Link with Android libraries
target_link_libraries(lang_it_jni
        android
        log
)
