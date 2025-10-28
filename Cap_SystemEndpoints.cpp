/*** Cap_SystemEndpoints.cpp */
#include "Spieluhr.h"


void Spieluhr::setupSystemEndpoints() {


  server.on("/config.json", HTTP_GET, [&]() {
    File file = LittleFS.open("/config.json", "r");
    if (!file) {
      server.send(404, "text/plain", "config.json nicht gefunden");
      return;
    }
    server.streamFile(file, "application/json");
    file.close();
  });

  server.on("/thema.css", HTTP_GET, [&]() {
    File file = LittleFS.open("/thema.css", "r");
    if (!file) {
      server.send(404, "text/plain", "thema.css nicht gefunden");
      return;
    }
    server.streamFile(file, "text/css");
    file.close();
  });

  server.on("/favicon.png", HTTP_GET, [&]() {
    File f = LittleFS.open("/favicon.png", "r");
    if (!f) {
      server.send(404, "text/plain", "Favicon nicht gefunden");
      return;
    }
    server.streamFile(f, "image/png");
    f.close();
  });

  server.on("/uhrzeit", HTTP_GET, [&]() {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);

    char buffer[6];  // HH:MM
    snprintf(buffer, sizeof(buffer), "%02d:%02d", t->tm_hour, t->tm_min);
    server.send(200, "text/plain", String(buffer));
  });

  server.on("/datum", HTTP_GET, [&]() {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);

    const char* wochentage[] = { "So.", "Mo.", "Di.", "Mi.", "Do.", "Fr.", "Sa." };
    const char* monate[] = { "Jan.", "Feb.", "März", "April", "Mai", "Juni",
                             "Juli", "Aug.", "Sep.", "Okt.", "Nov.", "Dez." };

    String datum = String(wochentage[t->tm_wday]) + ", " + String(t->tm_mday) + ". " + String(monate[t->tm_mon]) + " " + String(t->tm_year + 1900);

    server.send(200, "text/plain", datum);
  });

}
