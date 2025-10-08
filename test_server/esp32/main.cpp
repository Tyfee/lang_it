#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "lang_it.h"  // wherever it is

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

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