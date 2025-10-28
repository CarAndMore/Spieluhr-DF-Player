/*** Spieluhr.h: */
#pragma once
/*** Spieluhr.h:
 *  Hauptklasse der Spieluhr-Logik.
 * 
 *  Diese Klasse kapselt alle zentralen Komponenten der Spieluhr:
 *  - Webserver für die Benutzeroberfläche
 *  - MP3-Player zur Audiowiedergabe
 *  - PCF8574-Portexpander für LED- und Touchsteuerung
 *  - Wiedergabe-Scheduler für zeitgesteuerte Auslösung
 * 
 *  Sie initialisiert alle Module, verwaltet die Endpunkte und bietet
 *  eine zentrale `loop()`-Methode zur zyklischen Verarbeitung.
 */

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "UserDaten.h"

#include "Cap_Expander.h"
#include "Cap_WebServer.h"
#include "Cap_DFPlayer.h"
#include "PlaybackScheduler.h"

/**
 * @class Spieluhr
 * @brief Zentrale Steuerklasse für die Spieluhr.
 * 
 * Diese Klasse verbindet Webserver, MP3-Player, Portexpander und Scheduler
 * zu einer modularen, interaktiven Spieluhr mit Weboberfläche.
 */
class Spieluhr {
public:
  /*** Gemeinsamer PCF8574-Expander für LED und Touchsensoren. */
  IO_Expander* pcf8574 = nullptr;

  /*** Webserver-Modul zur Bereitstellung der HTML-Oberfläche. */
  cap_Webserver* web = nullptr;

  /*** MP3-Player-Modul zur Audiowiedergabe. */
  MP3Player* mp3 = nullptr;

  /*** Scheduler-Modul zur zeitgesteuerten Wiedergabe. */
  PlaybackScheduler* scheduler = nullptr;

  /**
   * Konstruktor der Spieluhr.
   * 
   * @param webServer Referenz auf den initialisierten ESP8266-Webserver
   */
  Spieluhr(ESP8266WebServer& webServer)
    : server(webServer) {}

  /**
   * Initialisiert alle Module der Spieluhr.
   * 
   * - Initialisiert den PCF8574-Expander
   * - Startet den MP3-Player
   * - Lädt Konfigurationen aus dem Dateisystem
   * - Registriert Web-Endpunkte
   * 
   * @param PCF_addr I²C-Adresse des PCF8574 (Standard: 0x20)
   */
  void begin(uint8_t PCF_addr = 0x20);


  /**
   * Haupt-Loop der Spieluhr.
   * 
   * Diese Methode sollte regelmäßig im `loop()` aufgerufen werden.
   * Sie verarbeitet:
   * - MP3-Player-Events
   * - Scheduler-Logik
   * - Touchsensoren und Output-Zustände
   */
  void loop();

  /**
   * Registriert alle Web-Endpunkte für die Spieluhr.
   * 
   * Diese Methode ruft intern die Setup-Funktionen für MP3, System, Expander und Dateisystem auf.
   */
  void webEndpoints();
private:
  /*** Referenz auf den ESP8266-Webserver zur Bereitstellung der Weboberfläche. */
  ESP8266WebServer& server;

  /**
   * Registriert die Endpunkte zur Steuerung des MP3-Players.
   * 
   * z. B. /play, /stop, /volume, /random
   */
  void setupMP3Endpoints();

  /**
   * Registriert allgemeine System-Endpunkte.
   * 
   * z. B. /info, /config, /status
   */
  void setupSystemEndpoints();

  /**
   * Registriert Endpunkte zur Steuerung des PCF8574-Expanders.
   * 
   * z. B. /setoutput, /getoutputs
   */
  void setupExpanderEndpoints();

  /**
   * Registriert Endpunkte für Dateisystemoperationen.
   * 
   * z. B. /upload, /format, /favicon
   */
  void setupFilesystemEndpoints();

  /**
   * Formatiert das LittleFS-Dateisystem.
   * 
   * Wird z. B. bei Erstinstallation oder Reset verwendet.
   */
  void format_FS();

  /**
   * Generiert die Konfigurationsdatei mit Standardwerten.
   * 
   * @return true bei Erfolg; false bei Fehler
   */
  bool gen_configFile();

  /**
   * Generiert die Theme-Datei mit Standardfarben und Layout.
   * 
   * @return true bei Erfolg; false bei Fehler
   */
  bool gen_ThemaFile();

  /**
   * Lädt die Konfiguration aus dem Dateisystem.
   * 
   * z. B. Wiedergabeintervall, Lautstärke, Theme
   */
  void loadConfig();

  /*** Bitmaske zum Ausschalten aller Outputs (P0–P3). */
  uint8_t io_all_OFF = 0x0;

  /*** Bitmaske zum Einschalten aller Outputs (P0–P3). */
  uint8_t io_all_ON = 0xf;

  /*** Logischer Zustand für "OFF" (LOW). */
  bool io_OFF = false;

  /*** Logischer Zustand für "ON" (HIGH). */
  bool io_ON = true;
};
