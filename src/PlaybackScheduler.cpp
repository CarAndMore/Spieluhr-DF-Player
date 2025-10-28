/*** PlaybackScheduler.cpp: */
#include "PlaybackScheduler.h"

PlaybackScheduler::PlaybackScheduler(MP3Player* player)
  : mp3(player) {}

void PlaybackScheduler::setConfig(const PlaybackConfig& cfg) {
  config = cfg;
}

void PlaybackScheduler::update() {
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);

  if (!config.enabled) return;
  if (!config.activeDays[t->tm_wday]) return;

  if ((t->tm_min % config.intervalMinutes == 0) && (t->tm_hour * 60 + t->tm_min != lastMinute)) {

    lastMinute = t->tm_hour * 60 + t->tm_min;

    uint16_t track = (t->tm_yday % 35) + 1;
    mp3->play(track);
    mp3->setStatus("Kalender-Song für Tag " + String(t->tm_yday + 1));
  }
}
