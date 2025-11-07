g++ -g -O0 -DALL ../../../src/lang_it.h ../lang_it_ui.cpp ../../../src/pairs/*.cpp -o translator.exe -mwindows `pkg-config --cflags --libs gtk+-3.0`
