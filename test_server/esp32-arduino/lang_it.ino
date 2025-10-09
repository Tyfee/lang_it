#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#define PT_EN
#include "lang_it.h" 

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const char indexPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>lang_it</title>
<style>
body{font-family:sans-serif;background:#f1f1f1;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}
#c{background:#fff;padding:20px;border-radius:8px;box-shadow:0 0 10px rgba(0,0,0,0.2);width:300px}
select,textarea{width:100%;margin:5px 0;padding:8px;font-size:14px;border-radius:4px;border:1px solid #ccc;box-sizing:border-box}
#o{margin-top:10px;padding:8px;background:#f7f7f7;border-radius:4px;min-height:40px;white-space:pre-wrap}
</style>
</head>
<body>
<div id="c">
<select id="lang"><option value="pt_en">PT→EN</option><option value="en_ja">EN→JA</option><option value="pt_ja">PT→JA</option></select>
<textarea id="txt" placeholder="Enter text..."></textarea>
<div id="o"></div>
<script>
let t;
const txt=document.getElementById('txt'),lang=document.getElementById('lang'),out=document.getElementById('o');
async function tr(){if(!txt.value)return;const r=await fetch(`/translate/${lang.value}`,{method:'POST',body:txt.value});const j=await r.json();out.textContent=j.translation;}
txt.addEventListener('input',()=>{clearTimeout(t);t=setTimeout(tr,300)});lang.addEventListener('change',tr);
</script>
</div>
</body>
</html>
)rawliteral";

WebServer server(80);

void sendJsonResponse(const std::string &translation, double duration_ms) {
  StaticJsonDocument<200> doc;
  doc["translation"] = translation;
  doc["time_ms"] = duration_ms;

  String response;
  serializeJson(doc, response);

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204); 
}

void handleRoot(){
  server.send(200, "text/html", indexPage);
}

void handlePtEn() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"no body\"}");
    return;
  }

  unsigned long start = micros();
  std::string translation = translate(server.arg("plain").c_str(), "pt", "en");
  unsigned long end = micros();

  sendJsonResponse(translation, (end - start) / 1000.0);
}

void handleEnJa() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"no body\"}");
    return;
  }

  unsigned long start = micros();
  std::string translation = translate(server.arg("plain").c_str(), "en", "ja");
  unsigned long end = micros();

  sendJsonResponse(translation, (end - start) / 1000.0);
}

void handlePtJa() {
  if (!server.hasArg("plain")) {
    server.send(400, "application/json", "{\"error\":\"no body\"}");
    return;
  }

  unsigned long start = micros();
  std::string temp = translate(server.arg("plain").c_str(), "pt", "en");
  std::string translation = translate(temp.c_str(), "en", "ja");
  unsigned long end = micros();

  sendJsonResponse(translation, (end - start) / 1000.0);
}


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  server.on("/translate/pt_en", HTTP_POST, handlePtEn);
  server.on("/translate/pt_en", HTTP_OPTIONS, handleOptions);

  server.on("/translate/en_ja", HTTP_POST, handleEnJa);
  server.on("/translate/en_ja", HTTP_OPTIONS, handleOptions);

  server.on("/translate/pt_ja", HTTP_POST, handlePtJa);
  server.on("/translate/pt_ja", HTTP_OPTIONS, handleOptions);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
