#include "httplib.h"
#include "../../lang_it.h"
#include <chrono>
#include "nlohmann/json.hpp" 
#include <iostream>
#include <string>

int main() {
    httplib::Server svr;
svr.Post("/translate/pt_en", [](const httplib::Request &req, httplib::Response &res) {
    auto start = std::chrono::high_resolution_clock::now();

    std::string input_text = req.body;
    std::string translation =  translate(input_text.c_str(), "pt", "en");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double duration_ms = duration_us / 1000.0;
    nlohmann::json j;
    j["translation"] = translation;
    j["time_ms"] = duration_ms;

    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(j.dump(), "application/json; charset=utf-8");
});
    svr.Options("/translate/pt_en", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

     svr.Post("/translate/en_ja", [](const httplib::Request &req, httplib::Response &res) {
            auto start = std::chrono::high_resolution_clock::now();

            std::string input_text = req.body;
            std::string translation =  translate(input_text.c_str(), "en", "ja");

            auto end = std::chrono::high_resolution_clock::now();
            auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            double duration_ms = duration_us / 1000.0;
            nlohmann::json j;
            j["translation"] = translation;
            j["time_ms"] = duration_ms;

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(j.dump(), "application/json; charset=utf-8");
    });

    svr.Options("/translate/en_ja", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

       svr.Post("/translate/pt_ja", [](const httplib::Request &req, httplib::Response &res) {
            auto start = std::chrono::high_resolution_clock::now();

            // pivoting
            std::string input_text = req.body;
            std::string translation_ =  translate(input_text.c_str(), "pt", "en");
            std::string translation =  translate(translation_.c_str(), "en", "ja");

            auto end = std::chrono::high_resolution_clock::now();
            auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            double duration_ms = duration_us / 1000.0;
            nlohmann::json j;
            j["translation"] = translation;
            j["time_ms"] = duration_ms;

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(j.dump(), "application/json; charset=utf-8");
    });

    svr.Options("/translate/pt_ja", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

    std::cout << "lang_it running on port 8080\n";
    svr.listen("0.0.0.0", 8080);
}
