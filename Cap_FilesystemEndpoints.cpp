/*** Cap_FilesystemEndpoints.cpp */
#include "Spieluhr.h"
File fsUploadFile;

void Spieluhr::setupFilesystemEndpoints() {
  server.on("/system8266/formatC", [&]() {
    format_FS();
  });

  server.on("/savecss", HTTP_POST, [&]() {
    if (!server.hasArg("plain")) {
      server.send(400, "text/plain", "Kein CSS empfangen");
      return;
    }
    File f = LittleFS.open("/thema.css", "w");
    if (!f) {
      server.send(500, "text/plain", "Fehler beim Öffnen");
      return;
    }
    f.print(server.arg("plain"));
    f.close();
    server.send(200, "text/plain", "CSS gespeichert");
  });

  server.on("/fsinfo", HTTP_GET, [&]() {
    FSInfo info;
    LittleFS.info(info);

    Dir dir = LittleFS.openDir("/");
    uint16_t count = 0;
    while (dir.next()) {
      count++;
    }

    size_t free = (info.totalBytes - info.usedBytes) >> 10;  // in KB

    String json = "{";
    json += "\"count\":" + String(count) + ",";
    json += "\"free\":" + String(free);
    json += "}";

    server.send(200, "application/json", json);
  });
  
  server.on("/schedulerconfig", HTTP_GET, [&]() {
    if (!scheduler) {
      server.send(500, "application/json", "{\"error\":\"Scheduler nicht initialisiert\"}");
      return;
    }

    const PlaybackConfig& cfg = scheduler->getConfig();

    String json = "";
    json += "{";
    json += "\"enabled\": " + String(cfg.enabled ? "true" : "false") + ",";
    json += "\"intervalMinutes\": " + String(cfg.intervalMinutes) + ",";
    json += "\"activeDays\": [";
    for (int i = 0; i < 7; i++) {
      json += (cfg.activeDays[i] ? "true" : "false");
      if (i < 6) json += ",";
    }
    json += "]}";

    server.send(200, "application/json", json);
  });

  server.on("/saveconfig", HTTP_POST, [&]() {
    if (!server.hasArg("plain")) {
      server.send(400, "text/plain", "Kein JSON empfangen");
      return;
    }

    String body = server.arg("plain");
    DynamicJsonDocument doc(512);
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
      server.send(400, "text/plain", "Ungültiges JSON");
      return;
    }

    PlaybackConfig cfg;
    cfg.enabled = doc["enabled"];
    cfg.intervalMinutes = doc["intervalMinutes"];
    for (int i = 0; i < 7; i++) {
      cfg.activeDays[i] = doc["activeDays"][i];
    }

    scheduler->setConfig(cfg);

    // Optional: in config.json speichern
    File file = LittleFS.open("/config.json", "w");
    if (file) {
      serializeJson(doc, file);
      file.close();
    }

    server.send(200, "text/plain", "Konfiguration gespeichert");
  });

  server.on(
    "/uploadfavicon", HTTP_POST, [this]() {
      server.send(200, "text/html", "Favicon hochgeladen.");
    },
    [this]() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        // Immer als /favicon.png speichern
        fsUploadFile = LittleFS.open("/favicon.png", "w");
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
      } else if (upload.status == UPLOAD_FILE_END) {
        if (fsUploadFile) fsUploadFile.close();
      }
    });

}