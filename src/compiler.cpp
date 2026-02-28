#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

uint8_t count_words(const std::string& s) {
    if (s.empty()) return 0;
    uint8_t count = 1;
    for (char c : s) {
        if (c == '_') {
            count++;
            if (count > 3) {
                throw std::runtime_error("Entry exceeds max 3 words");
            }
        }
    }
    return count;
}

template<typename T>
void write_u(std::ofstream& fs, T value) {
    fs.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

void write_cstring(std::ofstream& fs, const std::string& s) {
    fs.write(s.c_str(), s.size() + 1); // always null-terminated
}

void compile_from_buffer(const char* output_path,
                         const char* data,
                         size_t size);

void compile_from_file(const char* output_path,
                       const char* json_path)
{
    std::ifstream file(json_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open input file\n";
        return;
    }

    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    compile_from_buffer(output_path, buffer.data(), buffer.size());
}

void compile_from_buffer(const char* output_path,
                         const char* data,
                         size_t size)
{
    std::ofstream fs(output_path,
                     std::ios::binary | std::ios::trunc);

    if (!fs) {
        std::cerr << "Failed to open output file\n";
        return;
    }

    json j = json::parse(data, data + size);

    
    //metadata section 

    write_u(fs, (uint8_t)0xD0);
    auto metadata = j["metadata"];
    
    std::string from = metadata["from"];
     write_u(fs, (uint8_t)0xF0);
     write_u(fs, static_cast<uint8_t>(from.size()));   
     write_cstring(fs, from);      

     std::string to = metadata["to"];
     write_u(fs, (uint8_t)0xF1);
     write_u(fs, static_cast<uint8_t>(to.size()));                             
     write_cstring(fs, to);    

     
     //dictionary section
    write_u(fs, (uint8_t)0xD1);
    for (auto& item : j["dictionary"]) {

        std::string entry = item["entry"];
        std::string translation = item["translation"];
        uint8_t word_type = item["word_type"];

        // entry
        write_u(fs, (uint8_t)0xF0);
        write_u(fs, static_cast<uint8_t>(entry.size()));   
        write_u(fs, count_words(entry));                   
        write_u(fs, (uint8_t)0x00);                       
        write_cstring(fs, entry);                          

        // translation
        write_u(fs, (uint8_t)0xF1);
        write_u(fs, static_cast<uint8_t>(translation.size())); 
        write_u(fs, (uint8_t)0x00);                          
        write_cstring(fs, translation);                         

        // type
        write_u(fs, (uint8_t)0xF2);
        write_u(fs, word_type);
    }

    // rules section
    write_u(fs, (uint8_t)0xD2);

    // NORMALIZING rules section

    write_u(fs, (uint8_t)0xD3);
    for (auto& item : j["replace"]) {
        std::string original = item["original"];
        std::string target = item["target"];
        uint8_t where = item["where"];
        
        write_u(fs, (uint8_t)0xF0);
        write_u(fs, static_cast<uint8_t>(original.size()));                             
        write_cstring(fs, original);    

               
        write_u(fs, (uint8_t)0xF1);
        write_u(fs, static_cast<uint8_t>(target.size()));                             
        write_cstring(fs, target);  

        write_u(fs, (uint8_t)0xF2);
        write_u(fs, where);
                       
    }
    
    fs.close();
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: compiler output.bin input.json\n";
        return 1;
    }

    compile_from_file(argv[1], argv[2]);
}