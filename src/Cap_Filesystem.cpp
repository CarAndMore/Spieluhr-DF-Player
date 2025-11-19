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
    configFile.println("\"enabled\": true,");
    configFile.println("\"intervalHours\": 1,");
    configFile.println("\"startHour\": 8,");
    configFile.println("\"endHour\": 16,");
    configFile.println("\"outputModes\": [1, 1, 1, 1],");
    configFile.println("\"activeDays\": [true, true, true, true, true, true, true]");
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
        cfg.startHour = doc["startHour"] | 8;
        cfg.endHour = doc["endHour"] | 13;
        for (int i = 0; i < 4; i++) {
          cfg.outputModes[i] = doc["outputModes"][i] | 0;
        }
        for (int i = 0; i < 7; i++) {
          cfg.activeDays[i] = doc["activeDays"][i] | true;
        }
      }
    }
  } else {
    cfg.enabled = true;
    cfg.intervalMinutes = 60;
    for (int i = 0; i < 7; i++) cfg.activeDays[i] = true;
  }

  if (scheduler) scheduler->setConfig(cfg);
}

bool Spieluhr::gen_ThemaFile() {
  File cssFile = LittleFS.open("/thema.css", "w");
  uint8_t r = 128, g = 128, b = 255;
  String background = "#000022";
  String accent = "rgba(" + String(r) + "," + String(g) + "," + String(b) + ",1.0)";
  String accentSoft = "rgba(" + String(r) + "," + String(g) + "," + String(b) + ",0.4)";
  String accentVisited = "rgba(" + String(r) + "," + String(g) + "," + String(b) + ",0.6)";

  if (cssFile) {
    cssFile.println("body {\n\tbackground: " + background + ";\n\tcolor: #dddddd;\n}");

    cssFile.println("h1, a {\n\tcolor: " + accent + ";\n}");
    cssFile.println("a:visited {\n\tcolor: " + accentVisited + ";\n}");

    cssFile.println("footer, #infoTable th {\n\tbackground: " + background + ";");
    cssFile.println("\tborder-color: " + accent + ";\n\tcolor: #ffffff;\n}");

    cssFile.println("#infoTable td {\n\tborder-color: " + accent + ";\n}");

    cssFile.println("#volumeSlider::-webkit-slider-thumb {\n\tbackground: " + accent + ";\n}");
    cssFile.println("#volumeSlider::-moz-range-thumb {\n\tbackground: " + accent + ";\n}");
    cssFile.println("#volumeSlider::-webkit-slider-runnable-track {\n\tbackground: " + accentSoft + ";\n}");
    cssFile.println("#volumeSlider::-moz-range-track {\n\tbackground: " + accentSoft + ";\n}");

    cssFile.println(".audioBar {\n\tbackground: " + accentSoft + ";\n}");

    cssFile.println("input[type='file'] {\n\tcolor: " + accent + ";\n}");
    cssFile.println(".checkboxWrap {\n\tcolor: " + accent + ";\n}");
    cssFile.println(".checkboxCustom, .checkboxWrap input[type='checkbox']:checked + .checkboxCustom::after {\n\tbackground: " + background + ";\n\tborder-color: " + accent + ";\n}");
    cssFile.println(".btn, select,\n  input[type='number'],input[type='file']::file-selector-button {");
    cssFile.println("\tbackground: " + background + ";\n\tcolor: " + accent + ";\n\tborder-color: " + accentSoft + ";\n}");

    cssFile.close();
    return true;
  }
  return false;
}
