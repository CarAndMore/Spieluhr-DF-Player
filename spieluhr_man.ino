#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <time.h>

#include "UserDaten.h"  
#include "SpieluhrDFPlayer.h"


ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

// Instanz der Spieluhr-Klasse
SpieluhrDFPlayer spieluhr(MOSFET_LED, BUTTON_RND, server);  // LED-Pin + Webserver übergeben

void setup() {
  EEPROM.begin(2);  // 1 Byte für modi, 1 Byte für werkstattModus
  Serial.begin(9600);  // Muss aktiv sein für DFPlayer

  // WLAN-Verbindung aufbauen
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(APNAME, APPSK);
  wifiMulti.addAP(STASSID, STAPSK);
  wifiMulti.addAP(STASSID2, STAPSK2);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
  }

  // Webserver konfigurieren
  spieluhr.setupWebEndpoints();

  server.begin();

  // OTA konfigurieren
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(otaPASSWD);
  ArduinoOTA.begin();

  // Zeitsynchronisation
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  // Initialisierung der Spieluhr
  spieluhr.begin();

  // Warte auf gültige Zeit
  while (time(nullptr) < 100000) {
    delay(100);
  }

  // Startmelodie abspielen
  spieluhr.playStartupMelody();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  spieluhr.loop();  // Zeitgesteuerte Wiedergabe + DFPlayer-Handling
}
