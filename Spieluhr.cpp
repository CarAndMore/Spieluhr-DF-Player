/*** Spieluhr.cpp: */
#include "Spieluhr.h"
void Spieluhr::begin(uint8_t PCF_addr) {
  LittleFS.begin();

  pcf8574 = new IO_Expander(PCF_addr);
  pcf8574->begin();
  pcf8574->writeOutputs(io_all_OFF);

  mp3 = new MP3Player(Serial);
  mp3->begin(18);

  mp3->onPlaybackChange = [&](bool isPlaying) {
    if (pcf8574) {
      pcf8574->setOutputPin(0, isPlaying);
      pcf8574->setOutputPin(1, isPlaying);
      pcf8574->setOutputPin(2, isPlaying);
      pcf8574->setOutputPin(3, isPlaying);
    }
  };
  web = new cap_Webserver(server);
  scheduler = new PlaybackScheduler(mp3);
  loadConfig();
}

void Spieluhr::loop() {
  mp3->loop();

  if (scheduler) scheduler->update();

  for (int i = 0; i < 4; i++) {
    if (pcf8574->isRisingEdgeDebounced(i)) {
      if (i == 0) {
        mp3->playRandomSong();
      } else if (i == 1) {
        mp3->stop();
      } else if (i == 3) {
        mp3->play(32);
      }
    }
  }
}

void Spieluhr::webEndpoints() {
  web->setupWebEndpoints();
  setupMP3Endpoints(); // Cap_mp3ndpoints.cpp
  setupSystemEndpoints(); // Cap_SystemEndpoints.cpp
  setupExpanderEndpoints(); // Cap_ExpanderEndpoints.cpp
  setupFilesystemEndpoints(); // Cap_FilesystemEndpoints.cpp
}

