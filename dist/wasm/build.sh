em++ -DALL ./lang_it_wasm.cpp ../../src/pairs/*.cpp -o lang_it.js \
  -s MODULARIZE=1 \
  -s EXPORT_ES6=1 \
  -s ENVIRONMENT=web \
  -s SINGLE_FILE=1 \
  -lembind
