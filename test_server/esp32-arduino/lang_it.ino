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
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>lang_it (ESP32)</title>
<style>
*{box-sizing:border-box}
body{
  font-family:system-ui,Arial,sans-serif;
  background:#f2f2f2;
  margin:0;
  padding:0;
  display:flex;
  flex-direction:column;
  align-items:center;
  min-height:100vh;
}
header{
  background:#0078d7;
  color:#fff;
  width:100%;
  text-align:center;
  padding:10px 0;
  font-size:18px;
  box-shadow:0 2px 6px rgba(0,0,0,0.2);
}
#container{
  background:#fff;
  margin-top:40px;
  padding:20px;
  border-radius:10px;
  width:90%;
  max-width:400px;
  box-shadow:0 0 10px rgba(0,0,0,0.15);
}
select,textarea{
  width:100%;
  margin:8px 0;
  padding:10px;
  font-size:16px;
  border-radius:6px;
  border:1px solid #ccc;
  resize:vertical;
}
#output{
  margin-top:10px;
  padding:10px;
  background:#fafafa;
  border-radius:6px;
  min-height:50px;
  white-space:pre-wrap;
  border:1px solid #ddd;
}
@media (max-width:500px){
  header{font-size:16px;}
  textarea{font-size:15px;}
}
</style>
</head>
<body>
<header id="info">lang_it (ESP32)</header>
<div id="container">
  <select id="lang">
    <option value="pt_en">PT→EN</option>
    <option value="en_ja">EN→JA</option>
    <option value="pt_ja">PT→JA</option>
  </select>
  <textarea id="txt" rows="4" placeholder="Enter text..."></textarea>
  <div id="output"></div>
</div>
<script>
let t;
const txt=document.getElementById('txt'),
      lang=document.getElementById('lang'),
      out=document.getElementById('output');

async function translateNow(){
  if(!txt.value.trim())return;
  try{
    const r=await fetch(`/translate/${lang.value}`,{
      method:'POST',
      body:txt.value
    });
    const j=await r.json();
    out.textContent=j.translation||'(no result)';
  }catch(e){
    out.textContent='Error: '+e.message;
  }
}

txt.addEventListener('input',()=>{clearTimeout(t);t=setTimeout(translateNow,400)});
lang.addEventListener('change',translateNow);
</script>
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

void handleRoot() {
  String html(indexPage);
  html.replace("lang_it (ESP32)", 
    "lang_it (ESP32) — " + WiFi.SSID() + " / " + WiFi.localIP().toString());
  server.send(200, "text/html", html);
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

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
