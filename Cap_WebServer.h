/*** Cap_WebServer.h: */
#pragma once
/*** Cap_WebServer.h:
  *  Webserver-Modul der Spieluhr.
  * 
  *  Diese Klasse kapselt die HTTP-Endpunkt-Logik für die Weboberfläche der Spieluhr.
  *  Sie stellt HTML, CSS und JavaScript bereit, verarbeitet AJAX-Anfragen und liefert
  *  dynamische Inhalte wie Statusinformationen, Konfiguration und Steuerbefehle.
  * 
  *  Besonderheiten:
  *  - Modularer Aufbau: HTML, CSS und JS werden getrennt generiert
  *  - Dynamische Tabs: Infos, Player, Outputs, Konfig, Editor
  *  - Integration mit PCF8574, MP3-Player und Scheduler über AJAX-Endpunkte
  */
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "UserDaten.h"

/**
  * @class cap_Webserver
  * @brief Kapselt die Webserver-Endpunkte und UI-Generierung der Spieluhr.
  */
class cap_Webserver {
public:
  /**
    * Konstruktor für die Webserver-Klasse.
    * 
    * @param webServer Referenz auf den bereits initialisierten ESP8266WebServer
    */
  cap_Webserver(ESP8266WebServer& webServer)
    : server(webServer) {}

  /**
    * Registriert alle HTTP-Endpunkte für die Weboberfläche.
    * 
    * Dazu gehören:
    * - Root-Seite ("/", "/index.html", Captive Portal)
    * - Skript- und Stylesheet-Endpunkte ("/script.js", "/style.css")
    * - AJAX-Endpunkte für Status, Konfiguration, Audio, Outputs etc.
    */
  void setupWebEndpoints();


private:

  /**
    * Registriert alle HTTP-Endpunkte für die Weboberfläche.
    * 
    * Dazu gehören:
    * - Root-Seite ("/", "/index.html", Captive Portal)
    * - Skript- und Stylesheet-Endpunkte ("/script.js", "/style.css")
    * - AJAX-Endpunkte für Status, Konfiguration, Audio, Outputs etc.
    */
  void handleRoot();  // in "html.cpp"

  /**
   * Generiert den <head>-Bereich der HTML-Seite inkl. Meta, Titel, CSS, JS, Favicon.
   * 
   * @param title Seitentitel
   * @return HTML-String für den <head>-Bereich
   */
  String myHeader(String title);

  /**
   * Generiert die Audio-Steuerleiste mit Dropdown, Play/Stop/Random-Buttons.
   * 
   * @return HTML-String für den Audio-Bereich
   */
  String myAudioBar();

  /**
   * Generiert den Favicon-Upload-Bereich inkl. Vorschau und Upload-Button.
   * 
   * @return HTML-String für den Favicon-Uploader
   */
  String myFaviconUploader();

  /**
   * Generiert den CSS-Editor mit Textarea und Speichern/Neuladen-Buttons.
   * 
   * @return HTML-String für den Editor-Bereich
   */
  String myEditor();

  /**
   * Generiert den Lautstärkeregler als <input type="range">.
   * 
   * @return HTML-String für den Lautstärkebereich
   */
  String myVolume();

  /**
   * Generiert die Output-Schaltfläche für P0–P3 inkl. Statusanzeige.
   * 
   * @return HTML-String für den Output-Bereich
   */
  String myOutputControl();


  /**
   * Generiert die Output-Schaltfläche für P0–P3 inkl. Statusanzeige.
   * 
   * @return HTML-String für den Output-Bereich
   */
  void handleScriptJS();  // in "JavaScript.cpp"

  /**
   * Behandelt den Web-Endpunkt "/style.css" und liefert das CSS-Stylesheet für die Spieluhr-Webseite.
   * 
   * Das Stylesheet definiert:
   * - Layout, Schriftarten, Buttons, Tabellen
   * - Struktur für Tabs, Editor und AudioBar
   */
  void handleStyleCSS();  // in "Style.cpp"

  /**
   * Behandelt den Web-Endpunkt "/info" und liefert die Status-Tabelle als HTML.
   * 
   * Die Tabelle enthält:
   * - Uhrzeit, Datum, SSID, IP-Adresse
   * - Speicherplatz, Dateianzahl, Songanzahl, Playerstatus
   */
  void handleInfo();

  /**
   * Referenz auf den ESP8266-Webserver zur Bereitstellung der Weboberfläche.
   */
  ESP8266WebServer& server;
};
