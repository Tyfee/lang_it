cmake_minimum_required(VERSION 3.6)

project(lang_it)

add_library(lang_it_lib)

target_sources(lang_it_lib
   PRIVATE src/lang_it.h
   src/pairs/pt_en.cpp
   src/pairs/pt_es.cpp
   src/pairs/pt_sv.cpp
   src/pairs/en_pt.cpp
   src/pairs/en_ja.cpp
   src/pairs/zh_en.cpp
   src/pairs/pt_mbl.cpp
)

target_include_directories(lang_it_lib PUBLIC src)


target_compile_definitions(lang_it_lib PUBLIC PT_MBL)


target_compile_features(lang_it_lib PUBLIC cxx_std_17)

if (MSVC)
    target_compile_options(lang_it_lib PRIVATE /utf-8)
endif()

add_executable(lang_it)
target_compile_definitions(lang_it PUBLIC PT_MBL)
target_sources(lang_it
  PRIVATE lang_it.cpp
)

target_link_libraries(lang_it PRIVATE lang_it_lib)

install(TARGETS lang_it
    RUNTIME DESTINATION bin
)

