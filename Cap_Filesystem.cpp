/*** Cap_SystemEndpoints.cpp.cpp: */
#include "Spieluhr.h"

void Spieluhr::format_FS() {
  if (LittleFS.format()) {
    gen_configFile();
    gen_ThemaFile();
    server.send(200, "text/plain", "Dateisystem formatiert und Dateien erstellt.");
  } else {
    server.send(500, "text/plain", "Formatierung fehlgeschlagen.");
  }
}

bool Spieluhr::gen_configFile() {
  File configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    configFile.println("{");
    configFile.println("  \"enabled\": true,");
    configFile.println("  \"intervalHours\": 1,");
    configFile.println("  \"activeDays\": [true, true, true, true, true, true, true]");
    configFile.println("}");
    configFile.close();
    return true;
  }
  return false;
}

void Spieluhr::loadConfig() {
  PlaybackConfig cfg;

  if (LittleFS.exists("/config.json")) {
    File file = LittleFS.open("/config.json", "r");
    if (file) {
      DynamicJsonDocument doc(512);
      DeserializationError err = deserializeJson(doc, file);
      file.close();

      if (!err) {
        cfg.enabled = doc["enabled"] | true;
        cfg.intervalMinutes = doc["intervalMinutes"] | 60;
        for (int i = 0; i < 7; i++) {
          cfg.activeDays[i] = doc["activeDays"][i] | true;
        }
      } else {
        Serial.println("⚠️ Fehler beim Parsen von config.json – verwende Standardwerte");
      }
    }
  } else {
    Serial.println("ℹ️ config.json nicht gefunden – verwende Standardwerte");
    cfg.enabled = true;
    cfg.intervalMinutes = 60;
    for (int i = 0; i < 7; i++) cfg.activeDays[i] = true;
  }

  if (scheduler) scheduler->setConfig(cfg);
}

bool Spieluhr::gen_ThemaFile() {
  File cssFile = LittleFS.open("/thema.css", "w");
  uint8_t r = 170, g = 32, b = 32;

  if (cssFile) {
    cssFile.println("body {\n\tbackground: #555555;\n\tcolor: #111111;\n}");
    cssFile.println("h1 {\n\tcolor: rgba(" + String(r) + ", " + String(g) + ", " + String(b) + ", 1.0);\n}");

    cssFile.println("#volumeSlider::-webkit-slider-thumb {\n\tbackground: rgba(" + String(r) + ", " + String(g) + ", " + String(b) + ", 1.0);\n}");
    cssFile.println("#volumeSlider::-moz-range-thumb {\n\tbackground: rgba(" + String(r) + ", " + String(g) + ", " + String(b) + ", 1.0);\n}");
    cssFile.println("#volumeSlider::-webkit-slider-runnable-track {\n\tbackground: rgba(" + String(r) + ", " + String(g) + ", " + String(b) + ", 0.4);\n}");
    cssFile.println("#volumeSlider::-moz-range-track {\n\tbackground: rgba(" + String(r) + ", " + String(g) + ", " + String(b) + ", 0.4);\n}");

    cssFile.println(".audioBar {\n\tbackground: rgba(" + String(r) + "," + String(g) + "," + String(b) + ",0.4);\n}");
    cssFile.println(".audioBar select {\n\tbackground: #666666; color: #ffffff; }");

    cssFile.println(".btn {\n\tbackground: rgba(" + String(r) + "," + String(g) + "," + String(b) + ",1.0);;\n\tcolor: #ffffff; }\n");
    cssFile.println(".btn:hover {\n\tbackground: rgba(" + String(r) + "," + String(g) + "," + String(b) + ",0.5);\n}");

    cssFile.close();
    return true;
  }
  return false;
}
