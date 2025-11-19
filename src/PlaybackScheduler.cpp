/*** PlaybackScheduler.cpp: */
#include "PlaybackScheduler.h"

PlaybackScheduler::PlaybackScheduler(MP3Player* player, IO_Expander* expander)
  : mp3(player), _expander(expander) {}

void PlaybackScheduler::setConfig(const PlaybackConfig& cfg) {
  config = cfg;
}

void PlaybackScheduler::update() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  if (!config.enabled) return;
  if (!config.activeDays[t->tm_wday]) return;
  // Wenn außerhalb des Zeitfensters → Outputs zurücksetzen
  if (t->tm_hour < config.startHour || t->tm_hour > config.endHour) {
    for (int i = 0; i < 4; i++) {
      if (config.outputModes[i] == 2) {  // "immer an"
        setOutput(i, false);             // ausschalten
      }
    }
    return;
  }
  if ((t->tm_min % config.intervalMinutes == 0) && (t->tm_hour * 60 + t->tm_min != lastMinute)) {

    lastMinute = t->tm_hour * 60 + t->tm_min;

    // Tag im Monat (1–31) direkt als Tracknummer
    uint16_t track = t->tm_mday;

    mp3->play(track);
    mp3->setStatus("Kalender-Song für Tag " + String(track));
  }
}

void PlaybackScheduler::update_365() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  if (!config.enabled) return;
  if (!config.activeDays[t->tm_wday]) return;
  // Wenn außerhalb des Zeitfensters → Outputs zurücksetzen
  if (t->tm_hour < config.startHour || t->tm_hour > config.endHour) {
    for (int i = 0; i < 4; i++) {
      if (config.outputModes[i] == 2) {  // "immer an"
        setOutput(i, false);             // ausschalten
      }
    }
    return;
  }

  if ((t->tm_min % config.intervalMinutes == 0) && (t->tm_hour * 60 + t->tm_min != lastMinute)) {

    lastMinute = t->tm_hour * 60 + t->tm_min;

    uint16_t track = (t->tm_yday % 35) + 1;
    mp3->play(track);
    mp3->setStatus("Kalender-Song für Tag " + String(t->tm_yday + 1));
  }
}

void PlaybackScheduler::setOutput(uint8_t pin, bool state) {
  _expander->setOutputPin(pin, state);
}
