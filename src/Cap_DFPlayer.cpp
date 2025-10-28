/*** Cap_DFPlayer.cpp: */
#include "Cap_DFPlayer.h"

MP3Player* MP3Player::Mp3Notify::instance = nullptr;

void MP3Player::begin(uint8_t initialVolume) {
  randomSeed(analogRead(A0));  // A0 muß frei bleiben!
  Mp3Notify::instance = this;
  dfmp3.begin();
  setVolume(initialVolume);
  setStatus("Bereit");
}

void MP3Player::loop() {
  dfmp3.loop();
}

void MP3Player::stop() {
  dfmp3.stop();
  if (onPlaybackChange) onPlaybackChange(false);
  setStatus("Wiedergabe stoppen");
}

uint16_t MP3Player::play(uint16_t track) {
  dfmp3.setVolume(volume);
  dfmp3.playMp3FolderTrack(track);
  if (onPlaybackChange) onPlaybackChange(true);
  setStatus("Spiele Song: " + String(track));
  return track;
}

void MP3Player::setVolume(uint8_t vol) {
  volume = constrain(vol, 0, 30);
  dfmp3.setVolume(volume);
  setStatus("Lautstärke: " + String(volume));
}

uint16_t MP3Player::playRandomSong() {
  uint16_t total = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  if (total == 0) return 0;

  uint16_t song;
  uint8_t attempts = 0;

  do {
    song = random(1, total + 1);
    attempts++;
  } while (isInHistory(song) && attempts < 10);

  play(song);
  addToHistory(song);
  setStatus("Spiele Song: " + String(song));

  return song;
}

uint16_t MP3Player::getTrackCount() {
  return dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
}

void MP3Player::addToHistory(uint16_t song) {
  songHistory[historyIndex] = song;
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

bool MP3Player::isInHistory(uint16_t song) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    if (songHistory[i] == song) return true;
  }
  return false;
}

