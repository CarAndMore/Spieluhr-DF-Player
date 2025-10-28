/*** Cap_mp3ndpoints.cpp */
#include "Spieluhr.h"

void Spieluhr::setupMP3Endpoints() {
  server.on("/mp3status", [&]() {
    server.send(200, "text/plain", mp3->getStatus());
  });

  server.on("/play", HTTP_GET, [&]() {
    if (!server.hasArg("track")) {
      server.send(400, "text/plain", "Kein Track angegeben");
      return;
    }
    uint16_t track = server.arg("track").toInt();
    mp3->play(track);
    server.send(200, "text/plain", "Spiele Track " + String(track));
  });

  server.on("/random", [&]() {
    mp3->playRandomSong();
    server.send(200, "text/plain", "Play Random Song!");
  });

  server.on("/stop", [&]() {
    mp3->stop();
    server.send(200, "text/plain", "Stoppe Wiedergabe!");
  });

  server.on("/count", [&]() {
    uint16_t total = mp3->getTrackCount();
    server.send(200, "text/plain", String(total) + " Songs");
  });

  server.on("/volume", [&]() {
    server.send(200, "text/plain", String(mp3->getVolume()));
  });

  server.on("/setvolume", HTTP_GET, [&]() {
    if (!server.hasArg("value")) {
      server.send(400, "text/plain", "Kein Wert");
      return;
    }
    uint8_t vol = server.arg("value").toInt();
    mp3->setVolume(vol);
    server.send(200, "text/plain", "Lautstärke gesetzt: " + String(vol));
  });
}
