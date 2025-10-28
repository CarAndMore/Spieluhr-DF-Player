/*** Cap_DFPlayer.h: */
#pragma once
/*** Cap_DFPlayer.h:
 *  DFPlayer-Mini Steuerung für die Spieluhr.
 * 
 *  Diese Klasse kapselt die Kommunikation mit dem DFPlayer Mini MP3-Modul
 *  über die Makuna-Bibliothek `DFMiniMp3`. Sie bietet Funktionen zur
 *  Wiedergabe einzelner Tracks, Zufallswiedergabe, Lautstärkesteuerung
 *  und Statusabfrage.
 * 
 *  Besonderheiten:
 *  - Ringpuffer zur Vermeidung von Wiederholungen bei Zufallswiedergabe
 *  - Callback-Funktion bei Wiedergabeende (`onPlaybackChange`)
 *  - Integration mit `PlaybackScheduler` und Webserver-Endpunkten
 *  - Status-Text zur Anzeige im Webinterface
 */
#include <Arduino.h>
#include <DFMiniMp3.h>  // "DFPlayer Mini Mp3 by Makuna"


#define HISTORY_SIZE 20

/**
 * @class MP3Player
 * @brief Steuerung des DFPlayer Mini MP3-Moduls.
 */
class MP3Player {
public:
  /**
   * Callback-Funktion, die bei Wiedergabeänderung aufgerufen wird.
   * 
   * Parameter: `true` = Wiedergabe gestartet, `false` = Wiedergabe beendet
   */
  std::function<void(bool isPlaying)> onPlaybackChange;

  /**
   * Konstruktor für den MP3Player.
   * 
   * @param serial Referenz auf die serielle Schnittstelle zum DFPlayer (z. B. Serial1)
   */
  MP3Player(HardwareSerial& serial)
    : dfmp3(serial) {}

  /**
   * Initialisiert den DFPlayer und setzt die Startlautstärke.
   * 
   * @param initialVolume Lautstärke 0–30 (Standard: 14)
   */
  void begin(uint8_t initialVolume = 14);

  /**
   * Muss regelmäßig im `loop()` aufgerufen werden, um Player-Events zu verarbeiten.
   */
  void loop();

  /**
   * Muss regelmäßig im `loop()` aufgerufen werden, um Player-Events zu verarbeiten.
   */
  void stop();

  /**
   * Spielt einen bestimmten Track ab.
   * 
   * @param track Tracknummer (1-basiert)
   * @return Tracknummer bei Erfolg
   */
  uint16_t play(uint16_t track);

  /**
   * Spielt einen zufälligen Track, der nicht kürzlich abgespielt wurde.
   * 
   * @return Tracknummer bei Erfolg
   */
  uint16_t playRandomSong();

  /**
   * Gibt die aktuelle Lautstärke zurück.
   * 
   * @return Lautstärke 0–30
   */
  uint8_t getVolume() const {
    return volume;
  }

  /**
   * Setzt die Lautstärke.
   * 
   * @param vol Lautstärke 0–30
   */
  void setVolume(uint8_t vol);

  /**
   * Gibt die Anzahl verfügbarer Tracks auf dem Medium zurück.
   * 
   * @return Anzahl der Tracks
   */
  uint16_t getTrackCount();

  /**
   * Gibt den aktuellen Status-Text zurück (z. B. "Wiedergabe läuft").
   * 
   * @return Status-String
   */
  String getStatus() const {
    return player_Status;
  }

  /**
   * Setzt den Status-Text (z. B. für Webanzeige).
   * 
   * @param text Neuer Status
   */
  void setStatus(String text) {
    player_Status = text;
  }

private:
  /*** Interne Klasse zur Behandlung von DFPlayer-Benachrichtigungen (z. B. Track-Ende). */
  class Mp3Notify;

  /*** DFPlayer-Wrapper mit Benachrichtigungs-Callback. */
  typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;
  DfMp3 dfmp3;

  /*** Aktuelle Lautstärke (0–30). */
  uint8_t volume = 14;

  /*** Status-Text für Webanzeige oder Debug. */
  String player_Status = "unbekannt";

  /*** Medium-Status (z. B. USB, SD) – aktuell nicht verwendet. */
  String df_medium = "unbekannt";

  /*** Ringpuffer zur Speicherung der zuletzt gespielten Songs. */
  uint16_t songHistory[HISTORY_SIZE] = { 0 };

  /*** Aktueller Index im Ringpuffer `songHistory`. */
  uint8_t historyIndex = 0;

  /**
   * Fügt einen Song zur Wiedergabehistorie hinzu.
   * 
   * Der Song wird an der aktuellen Position im Ringpuffer `songHistory` gespeichert.
   * Der Index wird anschließend zyklisch weitergeschaltet, sodass ältere Einträge überschrieben werden,
   * sobald die maximale Historiengröße (`HISTORY_SIZE`) erreicht ist.
   * 
   * @param song Die Tracknummer, die zur Historie hinzugefügt werden soll.
   */
  void addToHistory(uint16_t song);

  /**
   * Prüft, ob ein bestimmter Song bereits in der Wiedergabehistorie enthalten ist.
   * 
   * Diese Funktion wird verwendet, um doppelte Wiedergaben zu vermeiden, z. B. bei Zufallswiedergabe.
   * 
   * @param song Die Tracknummer, die überprüft werden soll.
   * @return true, wenn der Song bereits in der Historie vorhanden ist, andernfalls false.
   */
  bool isInHistory(uint16_t song);

  /**
   * @class Mp3Notify
   * @brief Interne Callback-Klasse für DFPlayer-Ereignisse.
   * 
   * Diese Klasse verarbeitet Ereignisse wie Wiedergabeende und ruft ggf. `onPlaybackChange` auf.
   */
  class Mp3Notify {
  public:
    /*** Instanzzeiger für Zugriff auf MP3Player aus statischem Kontext. */
    static MP3Player* instance;

    /**
     * Wird aufgerufen, wenn ein Track zu Ende gespielt wurde.
     * 
     * @param track Die beendete Tracknummer
     */
    static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
      if (instance && instance->onPlaybackChange) {
        instance->onPlaybackChange(false);  // Wiedergabe beendet
        instance->setStatus("Wiedergabe wurde gestoppt");
      }
    }
    static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {}
    static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
    static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
  };
};
