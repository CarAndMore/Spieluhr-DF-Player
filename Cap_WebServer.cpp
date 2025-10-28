/*** Cap_WebServer.cpp: */
#include "Cap_WebServer.h"

void cap_Webserver::setupWebEndpoints() {
  server.on("/generate_204", [&]() {
    handleRoot();
  });
  server.on("/hotspot-detect.html", [&]() {
    handleRoot();
  });
  server.on("/redirect", [&]() {
    handleRoot();
  });
  server.on("/", [&]() {
    handleRoot();
  });
  server.on("/index.html", [&]() {
    handleRoot();
  });

  // Ergänzte Endpunkte:
  server.on("/script.js", [&]() {
    handleScriptJS();
  });
  server.on("/style.css", [&]() {
    handleStyleCSS();
  });

  server.on("/info", [&]() {
    handleInfo();
  });

  server.on("/iplocal", [&]() {
    server.send(200, "text/plain", WiFi.localIP().toString());
  });
  server.on("/ssid", [&]() {
    server.send(200, "text/plain", WiFi.SSID());
  });

}
