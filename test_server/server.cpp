#include "httplib.h"
#include "../lang_it.h"
#include <iostream>
#include <string>

int main() {
    httplib::Server svr;

    svr.Post("/translate/pt_en", [](const httplib::Request &req, httplib::Response &res) {
        std::string input_text = req.body;
        std::cout << "Received: " << input_text << std::endl;
        std::string translation = traduzir_en(input_text);

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(translation, "text/plain; charset=utf-8");
    });

    svr.Options("/translate/pt_en", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

     svr.Post("/translate/en_ja", [](const httplib::Request &req, httplib::Response &res) {
        std::string input_text = req.body;
        std::cout << "Received: " << input_text << std::endl;
        std::string translation = translate_ja(input_text);

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(translation, "text/plain; charset=utf-8");
    });

    svr.Options("/translate/en_ja", [](const httplib::Request &, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

    std::cout << "lang_it running on port 8080\n";
    svr.listen("0.0.0.0", 8080);
}
