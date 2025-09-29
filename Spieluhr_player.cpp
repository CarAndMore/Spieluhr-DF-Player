
/* Version 0.24 */
#include "SpieluhrDFPlayer.h"

/**
 * Initialisiert die Spieluhr-Hardware und lädt Konfigurationsdaten aus dem EEPROM.
 * 
 * Folgende Schritte werden durchgeführt:
 * - Setzen des LED-Pins (`MOSFET_LED`) auf OUTPUT und Ausschalten der LED
 * - Setzen des Button-Pins auf INPUT
 * - Laden der gespeicherten Einstellungen (`modi`, `werkstattModus`) aus dem EEPROM
 * - Initialisierung des DFPlayer Mini und Setzen der Lautstärke
 * - Setzen des internen Status auf "Bereit"
 * 
 * Diese Funktion sollte im `setup()`-Teil des Hauptprogramms aufgerufen werden.
 */
void SpieluhrDFPlayer::begin() {
  pinMode(MOSFET_LED, OUTPUT);
  digitalWrite(MOSFET_LED, 0);
  pinMode(Button, INPUT);
  EEPROM.begin(7);  // 7 Bytes: modi, werkstattModus, timeMode, startHH, startMM, endHH, endMM

  modi = EEPROM.read(0);
  werkstattModus = EEPROM.read(1);
  timeMode = EEPROM.read(2);
  startHH = EEPROM.read(3);
  startMM = EEPROM.read(4);
  endHH = EEPROM.read(5);
  endMM = EEPROM.read(6);

  EEPROM.end();

  randomSeed(analogRead(A0));  // A0 muß frei bleiben!

  dfmp3.begin();
  dfmp3.setVolume(volume);
  df_Status = "Bereit";
}

/**
 * Haupt-Loop der Spieluhr-Klasse, der regelmäßig aufgerufen werden sollte.
 * 
 * Diese Funktion übernimmt:
 * - `dfmp3.loop()`  → Wartung des DFPlayer Mini (z. B. Statusüberwachung)
 * - `checkAndPlayMp3()` → Zeitgesteuerte Wiedergabe basierend auf Modi und Uhrzeit
 * - `checkButton()`  → Überwachung des Hardware-Buttons für manuelle Wiedergabe
 * 
 * Sie sollte idealerweise in der `loop()`-Funktion des Hauptprogramms (Sketch) eingebunden werden.
 */
void SpieluhrDFPlayer::loop() {
  dfmp3.loop();
  checkAndPlayMp3();
  checkButton();
}

/**
 * Spielt die Startmelodie beim Hochfahren der Spieluhr.
 * 
 * Diese Funktion ruft `playDFplayer(32)` auf, wobei Track 32 als Begrüßungsmelodie definiert ist.
 * Sie kann z. B. im `begin()`-Setup oder direkt nach dem Bootvorgang verwendet werden.
 */
void SpieluhrDFPlayer::playStartupMelody() {
  playDFplayer(32);  // Startmelodie
}

/**
 * Überwacht den Hardware-Button und startet bei gültigem Tastendruck die Wiedergabe eines zufälligen Songs.
 * 
 * Die Funktion verwendet ein Entprellverfahren (Debounce), um versehentliche Mehrfachauslösungen zu vermeiden.
 * Sobald ein stabiler HIGH-Zustand erkannt wird, wird `playRandomSong()` aufgerufen.
 * 
 * Diese Funktion sollte regelmäßig im Hauptloop ausgeführt werden.
 */
void SpieluhrDFPlayer::checkButton() {
  bool reading = digitalRead(BUTTON_RND);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        playRandomSong();
      }
    }
  }

  lastButtonState = reading;
}

/**
 * Spielt einen bestimmten Song über den DFPlayer Mini ab und aktualisiert den Status.
 * 
 * Die Funktion setzt die Lautstärke, startet die Wiedergabe des angegebenen Tracks aus dem MP3-Ordner
 * und aktiviert die LED zur visuellen Rückmeldung. Der interne Statusstring wird entsprechend aktualisiert.
 * 
 * @param song Die Tracknummer, die abgespielt werden soll (1-basiert).
 * @return Die Tracknummer, die tatsächlich abgespielt wurde.
 */
uint16_t SpieluhrDFPlayer::playDFplayer(uint16_t song) {
  dfmp3.setVolume(volume);
  dfmp3.playMp3FolderTrack(song);
  df_Status = "Spiele Song: " + String(song);
  digitalWrite(MOSFET_LED, 1);
  return song;
}

/**
 * Überprüft regelmäßig die aktuelle Uhrzeit und spielt bei passenden Zeitpunkten automatisch einen Song ab.
 * 
 * Die Funktion wird einmal pro Sekunde im Hauptloop aufgerufen und prüft:
 * - Ob der Werkstatt-Modus aktiv ist (werktags oder immer)
 * - Ob ein neuer Minutenwechsel stattgefunden hat
 * - Ob der aktuelle Zeitpunkt mit dem gewählten Wiedergabemodus übereinstimmt:
 *   - Modi 0: alle 60 Minuten (volle Stunde)
 *   - Modi 1: alle 30 Minuten
 *   - Modi 2: alle 15 Minuten
 * 
 * Wenn alle Bedingungen erfüllt sind, wird ein Song abgespielt, dessen Tracknummer dem aktuellen Kalendertag entspricht.
 */
void SpieluhrDFPlayer::checkAndPlayMp3() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  static int lastMinute = -1;
  int minute = timeinfo->tm_min;
  int second = timeinfo->tm_sec;
  int day = timeinfo->tm_mday;
  int wochentag = timeinfo->tm_wday;

  bool werkstattAktiv = (werkstattModus == 0) || (werkstattModus == 1 && wochentag >= 1 && wochentag <= 5);
  if (!werkstattAktiv || minute == lastMinute || second != 0) return;
  lastMinute = minute;

  bool shouldPlay = false;
  switch (modi) {
    case 0:
      if (minute == 0) shouldPlay = true;
      break;
    case 1:
      if (minute == 0 || minute == 30) shouldPlay = true;
      break;
    case 2:
      if (minute % 15 == 0) shouldPlay = true;
      break;
  }

  if (shouldPlay) {
    if (isWithinTimeWindow()) {
      playDFplayer(day);  // Tag = Tracknummer
    }
  }
}

/**
 * Spielt einen zufälligen Song vom DFPlayer Mini ab, der nicht kürzlich in der Wiedergabehistorie vorkam.
 * 
 * Die Funktion ermittelt die Gesamtanzahl verfügbarer Tracks und wählt per Zufall einen Song aus.
 * Falls der Song bereits in der Historie enthalten ist, wird bis zu 20-mal ein neuer Versuch unternommen.
 * Der ausgewählte Song wird anschließend abgespielt und zur Historie hinzugefügt.
 * 
 * Diese Logik verhindert Wiederholungen und sorgt für abwechslungsreiche Wiedergabe.
 */
void SpieluhrDFPlayer::playRandomSong() {
  uint16_t total = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  if (total == 0) return;

  uint16_t song;
  uint8_t attempts = 0;

  do {
    song = random(1, total + 1);
    attempts++;
  } while (isInHistory(song) && attempts < 20);

  playDFplayer(song);
  addToHistory(song);
}

/**
 * Fügt einen Song zur Wiedergabehistorie hinzu.
 * 
 * Der Song wird an der aktuellen Position im Ringpuffer `songHistory` gespeichert.
 * Der Index wird anschließend zyklisch weitergeschaltet, sodass ältere Einträge überschrieben werden,
 * sobald die maximale Historiengröße (`HISTORY_SIZE`) erreicht ist.
 * 
 * @param song Die Tracknummer, die zur Historie hinzugefügt werden soll.
 */
void SpieluhrDFPlayer::addToHistory(uint16_t song) {
  songHistory[historyIndex] = song;
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

/**
 * Prüft, ob ein bestimmter Song bereits in der Wiedergabehistorie enthalten ist.
 * 
 * Diese Funktion wird verwendet, um doppelte Wiedergaben zu vermeiden, z. B. bei Zufallswiedergabe.
 * 
 * @param song Die Tracknummer, die überprüft werden soll.
 * @return true, wenn der Song bereits in der Historie vorhanden ist, andernfalls false.
 */
bool SpieluhrDFPlayer::isInHistory(uint16_t song) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    if (songHistory[i] == song) return true;
  }
  return false;
}

bool SpieluhrDFPlayer::isWithinTimeWindow() {
  if (timeMode == 0) return true;

  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  int currentMinutes = t->tm_hour * 60 + t->tm_min;
  int startMinutes = startHH * 60 + startMM;
  int endMinutes = endHH * 60 + endMM;

  return currentMinutes >= startMinutes && currentMinutes <= endMinutes;
}

/************************/
