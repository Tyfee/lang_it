em++ -DALL ./lang_it_wasm.cpp ../../src/pairs/*.cpp -o lang_it.js \
  -s MODULARIZE=1 \
  -s EXPORT_NAME="importTranslator" \
  -s ENVIRONMENT=web \
  -lembind
