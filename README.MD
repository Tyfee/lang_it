Langit means sky in some austronesian languages (malay being one i can speak).

Current WSL binary is at 172kb including libraries. My goal is to be able to achieve C1 translation between PT and a different language with a compiled binary with less than 500kb. 

## why would i do that to myself

Starting some serious studies about computational linguistics and machine translation, i wanted to know how far i can get with small amounts of code for a translation program based on rules on lemmas and stems.

My benchmarks will be:
- The book "O pequeno principe". (Full translation from PT-EN) ( ?? %);
- The song "We're all gonna die" by AJJ. (Full translation from EN-PT) ( ?? %);

At first, My goal is to introduce the following translations, and then pivot them between each other, so that any can be translated to any other:

| FROM          | TO            | STATUS | BINARY SIZE |
| ------------- | ------------- | ------ | ----------- |
| Portuguese    | English       |        |    187kb    |
| English       | Portuguese    |        |             |
| English       | French        |        |             |
| Spanish       | Portuguese    |        |             |
| Swedish       | English       |        |             |