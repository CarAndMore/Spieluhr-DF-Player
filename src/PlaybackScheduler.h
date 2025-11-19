/*** PlaybackScheduler.h: */
#pragma once
/*** PlaybackScheduler.h:
 *  Zeitbasierter Wiedergabe-Scheduler für die Spieluhr.
 * 
 *  Diese Klasse steuert die automatische MP3-Wiedergabe basierend auf einem konfigurierbaren Zeitintervall
 *  und aktiven Wochentagen. Sie prüft regelmäßig die aktuelle Uhrzeit und löst bei passenden Bedingungen
 *  eine Wiedergabe über den MP3Player aus.
 * 
 *  Besonderheiten:
 *  - Intervallsteuerung in Minuten (z.B. alle 60 Minuten)
 *  - Wochentagsfilter (z. B. nur Mo–Fr aktiv)
 *  - Integration mit `MP3Player` zur Triggerung der Wiedergabe
 *  - Leicht erweiterbar für z.B. Zeitfenster, Zufallsmodus oder Touch-Trigger
 */

#include <time.h>
#include "Cap_DFPlayer.h"
#include "Cap_Expander.h"

/**
 * @struct PlaybackConfig
 * @brief Konfigurationsstruktur für den Wiedergabe-Scheduler.
 * 
 * Diese Struktur enthält:
 * - Aktivierungsstatus (`enabled`)
 * - Intervall in Minuten (`intervalMinutes`)
 * - Wochentagsfilter (`activeDays[0–6]`, 0 = Sonntag)
 */
struct PlaybackConfig {
  bool enabled = true;

  uint8_t startHour = 8;  // Beginn des aktiven Zeitfensters
  uint8_t endHour = 16;   // Ende des aktiven Zeitfensters
  /**
   * Wiedergabeintervall in Minuten.
   * Beispiel: 60 = jede Stunde, 120 = alle 2 Stunden
   */
  uint8_t intervalMinutes = 60;  // 1 = jede Stunde, 2 = alle 2h, 4 = alle 4h

  /**
   * Aktivitätsstatus für jeden Wochentag (0 = Sonntag, 6 = Samstag).
   * true = aktiv, false = deaktiviert
   */
  bool activeDays[7] = { true, true, true, true, true, true, true };  // So–Sa

  uint8_t outputModes[4] = { 0, 0, 0, 0 };  // 0 = aus, 1 = bei Wiedergabe, 2 = immer an
};
/**
  * @class PlaybackScheduler
  * @brief Zeitgesteuerter Wiedergabe-Scheduler für MP3-Auslösung.
  * 
  * Diese Klasse prüft regelmäßig die aktuelle Uhrzeit und löst bei passenden Bedingungen
  * eine MP3-Wiedergabe aus. Die Konfiguration erfolgt über `PlaybackConfig`.
  */
class PlaybackScheduler {
public:
  /**
   * Konstruktor für den Scheduler.
   * 
   * @param player Zeiger auf das MP3Player-Objekt zur Steuerung der Wiedergabe
   */
  PlaybackScheduler(MP3Player* player, IO_Expander* expander);

    /**
   * Haupt-Update-Methode, sollte regelmäßig im `loop()` aufgerufen werden.
   * 
   * Prüft:
   * - Ist der Scheduler aktiviert?
   * - Ist der aktuelle Tag aktiv?
   * - Ist ein neues Intervall erreicht?
   * 
   * Bei Erfolg wird eine MP3-Wiedergabe ausgelöst.
   */
    void update();
  void update_365();

  void setOutput(uint8_t pin, bool state);  // <- Deklaration
  /**
   * Setzt die aktuelle Scheduler-Konfiguration.
   * 
   * @param cfg Neue Konfiguration (enabled, interval, activeDays)
   */
  void setConfig(const PlaybackConfig& cfg);

  /**
   * Gibt die aktuelle Konfiguration zurück.
   * 
   * @return Referenz auf die aktuelle `PlaybackConfig`
   */
  const PlaybackConfig& getConfig() const {
    return config;
  }
private:
  /**
   * Zeiger auf das MP3Player-Objekt zur Auslösung der Wiedergabe.
   */
  MP3Player* mp3;

  /**
   * Aktuelle Konfiguration des Schedulers.
   */
  PlaybackConfig config;

  /**
   * Letzter geprüfter Minutenwert (0–59).
   * Dient zur Erkennung von Intervallwechseln.
   */
  int lastMinute = -1;
  IO_Expander* _expander;   // jetzt Pointer statt Referenz
};
