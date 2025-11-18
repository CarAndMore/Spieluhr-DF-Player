/*** main.ino: */
/*** Spieluhr V4.2:
 *  Einstiegspunkt der Spieluhr-Firmware.
 * 
 *  Initialisiert alle Module (Webserver, MP3-Player, Portexpander, Scheduler),
 *  stellt die WLAN-Verbindung her, konfiguriert OTA-Updates und synchronisiert die Uhrzeit.
 * 
 *  Hauptfunktionen:
 *  - `setup()` → Initialisierung
 *  - `loop()` → zyklische Verarbeitung
 * 
 *  Dateienstruktur des Programms:
 *  - UserDaten.h
 *  - Spieluhr.h / Spieluhr.cpp / Cap_Filesystem.cpp /
 *    Cap_SystemEndpoints.cpp / Cap_mp3ndpoints.cpp / 
 *    Cap_ExpanderEndpoints.cpp / Cap_FilesystemEndpoints.cpp /
 *    OutputController.h
 *  - PlaybackScheduler.h / PlaybackScheduler.cpp
 *  - Cap_DFPlayer.h / Cap_DFPlayer.cpp
 *  - Cap_Expander.h / Cap_Expander.cpp
 *  - Cap_WebServer.h / Cap_WebServer.cpp / html.cpp / 
 *    JavaScript.cpp / Style.cpp
 */    

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

#include <ArduinoOTA.h>
#include <time.h>

#include "UserDaten.h"
#include "Spieluhr.h"



// Ports für DNS und Webserver
#define PORT_AP_DNS 53          ///< DNS-Port für Captive Portal 
#define PORT_HTML   80          ///< HTTP-Port für Webserver

// DNS-Server für Captive Portal
DNSServer dnsServer;

// WLAN-Multi für mehrere Zugangspunkte
ESP8266WiFiMulti wifiMulti;

// Webserver auf definiertem Port
ESP8266WebServer server(PORT_HTML);

// Hauptinstanz der Spieluhr
Spieluhr SpU(server);

// Funktionsprototypen
void my_init_Netzwerk();
void my_loop_Server();
void setupTime();

/**
 * Setup-Funktion der Spieluhr.
 * 
 * Initialisiert:
 * - Spieluhr-Module (Expander, MP3, Webserver)
 * - WLAN-Verbindung und Captive Portal
 * - OTA-Update
 * - Zeit-Synchronisation via NTP
 */
void setup() {
  SpU.begin(I2C_ADDR_PCF8574);
  my_init_Netzwerk();
  setupTime();
}

/**
 * Haupt-Loop der Spieluhr.
 * 
 * Verarbeitet:
 * - HTTP-Anfragen
 * - OTA-Updates
 * - DNS-Anfragen
 * - Spieluhr-Logik (MP3, Scheduler, Touch)
 */
void loop() {
  my_loop_Server();
  SpU.loop();
}

/**
 * Verarbeitet Webserver, OTA und DNS.
 * 
 * Wird zyklisch im Hauptloop aufgerufen.
 */
void my_loop_Server() {
  server.handleClient();
  ArduinoOTA.handle();
  dnsServer.processNextRequest();
}

/**
 * Initialisiert WLAN, Captive Portal, Webserver und OTA.
 * 
 * - Verbindet mit mehreren bekannten Netzwerken
 * - Startet Access Point
 * - Konfiguriert DNS-Umleitung
 * - Registriert Web-Endpunkte
 * - Startet OTA-Update-Modul
 */
void my_init_Netzwerk() {
  // WLAN-Verbindung aufbauen
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(APNAME, APPSK);
  wifiMulti.addAP(STASSID, STAPSK);
  wifiMulti.addAP(STASSID2, STAPSK2);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
  }

  dnsServer.start(PORT_AP_DNS, "*", WiFi.softAPIP());

  // Webserver konfigurieren
  SpU.webEndpoints();

  server.begin();

  // OTA konfigurieren
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(otaPASSWD);
  ArduinoOTA.begin();
  SpU.mp3->play(32);
}

/**
 * Konfiguriert die Zeit via NTP.
 * 
 * - NTP-Server: pool.ntp.org, time.nist.gov
 * - Zeitzone: Mitteleuropäische Zeit (CET/CEST)
 */
void setupTime() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  // Optional: Zeitzone setzen
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
}
