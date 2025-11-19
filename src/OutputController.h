/*** OutputController.h: */
#pragma once
#include <time.h>
#include "Cap_Expander.h"

class OutputController {
public:
  OutputController(IO_Expander* expander)
    : io(expander) {}

  void applyModes(const uint8_t modes[4], bool isPlaying, uint8_t startHour, uint8_t endHour) {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    bool isInTimeWindow = (t->tm_hour >= startHour && t->tm_hour < endHour);

    for (int i = 0; i < 4; i++) {
      switch (modes[i]) {
        case 0: io->setOutputPin(i, LOW); break;                          // Immer aus
        case 1: io->setOutputPin(i, isPlaying ? HIGH : LOW); break;       // Nur bei Wiedergabe
        case 2: io->setOutputPin(i, isInTimeWindow ? HIGH : LOW); break;  // Tagsüber an
      }
    }
  }

private:
  IO_Expander* io;
};
