#include "../../src/compiler.hpp"
#include <emscripten/bind.h>
#include <vector>
#include <cstdio>

std::vector<uint8_t> compile_json(std::string json_string) {
    const char* output_path = "output.bin";
    compile_from_buffer(output_path, json_string.c_str(), json_string.size());
    
    FILE* file = fopen(output_path, "rb");
    if (!file) return {};
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    std::vector<uint8_t> result(size);
    fread(result.data(), 1, size, file);
    fclose(file);
    
    remove(output_path);
    return result;
}

EMSCRIPTEN_BINDINGS(compiler) {
    emscripten::register_vector<uint8_t>("Uint8Array");
    emscripten::function("compile", &compile_json);
}