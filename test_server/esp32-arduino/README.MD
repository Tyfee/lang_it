["lang_it running on a local esp32 server"](./esp32_lang_it.jpg)
*short sentence takes 4ms to translate.*

* HOW TO: 

- You have to move the header file (lang_it.h) and the pairs you want (e.g: pt_en.cpp) to this folder along with your .ino file;
["project folder"](./filetree.png)

- Install the needed libraries: Wifi, WebServer and ArduinoJson

- Populate it with your wifi network info:

```cpp
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
```

- Add the defines on top of the file based on the pairs you want:

```cpp
#define PT_EN
#define JA_PT
#include "lang_it.h" //needs to come after
```

- Upload it with Arduino IDE.

- Send a POST request to the local server: 

http://[IP_ADRESS]/translate/from_to
