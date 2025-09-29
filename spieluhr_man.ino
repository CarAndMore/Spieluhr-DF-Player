/**
 * @file SpieluhrMain.ino
 * @brief Hauptprogramm für die Netzwerk-Spieluhr mit DFPlayer Mini und Webinterface.
 * 
 * Version: 0.23  
 * Autor: Carsten  
 * 
 * Dieses Programm steuert eine vernetzte MP3-Spieluhr auf Basis des ESP8266.
 * Es verbindet sich mit bekannten WLANs, stellt eine Weboberfläche bereit,
 * synchronisiert die Uhrzeit per NTP und spielt zu definierten Zeitpunkten Musik ab.
 * 
 * Funktionen:
 * - Dual-WLAN-Modus (Access Point + Station)
 * - Webserver mit konfigurierbaren Endpunkten
 * - OTA-Updates über ArduinoOTA
 * - Zeitgesteuerte Wiedergabe basierend auf Modi
 * - Manuelle Wiedergabe per Hardware-Button
 * - Startmelodie beim Hochfahren
 * 
 * Abhängigkeiten:
 * - DFPlayer Mini Library
 * - https://github.com/Makuna/DFMiniMp3/
 *
 * Weitere Konfigurationen (z. B. WLAN-Zugangsdaten, Hostname, Pins) befinden sich in `UserDaten.h`.
 */

#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>


#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <time.h>

#include "UserDaten.h"
#include "SpieluhrDFPlayer.h"


const byte DNS_PORT = 53;

DNSServer dnsServer;
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

// Instanz der Spieluhr-Klasse
SpieluhrDFPlayer spieluhr(server);  // LED-Pin + Webserver übergeben

/**
 * Initialisiert die Netzwerkverbindung, den Webserver und die OTA-Funktionalität.
 * 
 * Ablauf:
 * - Aktiviert den Dual-Modus (Access Point + Station) für WLAN
 * - Startet einen eigenen Access Point mit `APNAME` und `APPSK`
 * - Verbindet sich mit bis zu zwei bekannten WLANs über `wifiMulti`
 * - Wartet auf erfolgreiche Verbindung im Station-Modus
 * - Initialisiert die Web-Endpunkte der Spieluhr über `setupWebEndpoints()`
 * - Startet den Webserver
 * - Konfiguriert OTA (Over-the-Air) Updates mit Hostnamen und Passwort
 * 
 * Diese Funktion sollte im `setup()`-Teil des Hauptprogramms aufgerufen werden.
 */
void init_Netzwerk() {
  // WLAN-Verbindung aufbauen
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(APNAME, APPSK);
  wifiMulti.addAP(STASSID, STAPSK);
  wifiMulti.addAP(STASSID2, STAPSK2);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
  }

  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Webserver konfigurieren
  spieluhr.setupWebEndpoints();



  server.begin();

  // OTA konfigurieren
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(otaPASSWD);
  ArduinoOTA.begin();
}

void setup() {
  EEPROM.begin(2);     // 1 Byte für modi, 1 Byte für werkstattModus
  Serial.begin(9600);  // Muss aktiv sein für DFPlayer

  // Initialisiert die Netzwerkverbindung, den Webserver und die OTA-Funktionalität.
  init_Netzwerk();

  // Initialisierung der Spieluhr
  spieluhr.begin();

  // Zeitsynchronisation
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

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
  dnsServer.processNextRequest();
  spieluhr.loop();  // Zeitgesteuerte Wiedergabe + DFPlayer-Handling
}
