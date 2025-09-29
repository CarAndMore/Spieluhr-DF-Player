#pragma once
/* Version 0.23 */
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <time.h>
#include <ESP8266WebServer.h>
#include <DFMiniMp3.h>
#include <pgmspace.h>

#include "UserDaten.h"

#define START_VOLUME 12

class SpieluhrDFPlayer {
public:

   /**
    * Konstruktor für die Spieluhr-Klasse ohne LED- oder Button-Pin.
    * 
    * Initialisiert den Webserver und den DFPlayer Mini mit der seriellen Schnittstelle.
    * Diese Variante eignet sich für rein webbasierte Steuerung ohne Hardware-Interaktion.
    * 
    * @param webServer Referenz auf den bereits initialisierten ESP8266WebServer.
    */
  SpieluhrDFPlayer(ESP8266WebServer& webServer)
    : server(webServer), dfmp3(Serial) {}

  void begin();
  void loop();
  void playStartupMelody();
  void setupWebEndpoints();
private:
  /*** Pin-Nummer des Hardware-Buttons zur manuellen Wiedergabe. */
  uint8_t Button;

  /*** Wiedergabemodus: - 0: nur zur vollen Stunde  - 1: alle 30 Minuten  - 2: alle 15 Minuten. */
  uint8_t modi = 1;
  /*** Aktiviert das zeitbasierte Wiedergabefenster (1 = aktiv, 0 = deaktiviert). */
  uint8_t timeMode = 0;

  /*** Startzeit für das Wiedergabefenster (Stunde und Minute). */
  uint8_t startHH = 8;
  uint8_t startMM = 0;

  /*** Endzeit für das Wiedergabefenster (Stunde und Minute). */
  uint8_t endHH = 18;
  uint8_t endMM = 0;
  /*** Werkstatt-Modus:
   * - 0: immer aktiv
   * - 1: nur werktags (Mo–Fr)
   */
  uint8_t werkstattModus = 0;

  /*** Lautstärke für den DFPlayer Mini (0–30). */
  uint8_t volume = START_VOLUME;

  /*** Statusmeldung zur aktuellen Wiedergabe oder Systemzustand. */
  String df_Status = "unbekannt";

  /*** Aktueller Zustand des Buttons (HIGH oder LOW). */
  bool buttonState = LOW;

  /*** Letzter erkannter Zustand des Buttons (für Entprellung). */
  bool lastButtonState = LOW;

  /*** Zeitstempel der letzten Zustandsänderung des Buttons (für Entprellung). */
  unsigned long lastDebounceTime = 0;

  /*** Ringpuffer zur Speicherung der zuletzt gespielten Songs. */
  uint16_t songHistory[HISTORY_SIZE] = { 0 };

  /*** Aktueller Index im Ringpuffer `songHistory`. */
  uint8_t historyIndex = 0;

  /*** Referenz auf den ESP8266-Webserver zur Bereitstellung der Weboberfläche. */
  ESP8266WebServer& server;

  /*** Interne Klasse zur Behandlung von DFPlayer-Benachrichtigungen (z. B. Track-Ende). */
  class Mp3Notify;

  /*** Typdefinition für den DFPlayer Mini mit HardwareSerial und Mp3Notify. */
  typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;

  /*** Instanz des DFPlayer Mini zur Steuerung der MP3-Wiedergabe. */
  DfMp3 dfmp3;

  void playRandomSong();
  uint16_t playDFplayer(uint16_t song);

  void checkButton();
  void checkAndPlayMp3();

  void handleRoot();
  void handlePlay();
  void handleConfig();
  void handleScriptJS();
  void handleStyleCSS();
  void handleTimeNTP();
  void handleDatumNTP();

  void addToHistory(uint16_t song);
  bool isInHistory(uint16_t song);

  bool isWithinTimeWindow();
  String kleinerNull(int value);


  class Mp3Notify {
  public:
    static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
      digitalWrite(MOSFET_LED, 0);
    }
    static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {}
    static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
    static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
  };
};
