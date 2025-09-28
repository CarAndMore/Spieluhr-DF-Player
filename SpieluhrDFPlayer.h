#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <DFMiniMp3.h>
#include <time.h>
#include <ESP8266WebServer.h>

class SpieluhrDFPlayer {
  public:
    SpieluhrDFPlayer(uint8_t ledPin, ESP8266WebServer& webServer)
      : led(ledPin), server(webServer), dfmp3(Serial) {}

    void begin() {
      pinMode(led, OUTPUT);
      digitalWrite(led, HIGH);

      EEPROM.begin(2);
      modi = EEPROM.read(0);
      werkstattModus = EEPROM.read(1);
      EEPROM.end();

      dfmp3.begin();
      dfmp3.setVolume(volume);
      df_Status = "Bereit";

      digitalWrite(led, LOW);
    }

    void loop() {
      dfmp3.loop();
      checkAndPlayMp3();
    }

    void playStartupMelody() {
      playDFplayer(32);  // Startmelodie
    }

    void setupWebEndpoints() {
      server.on("/", [&]() {
        handleRoot();
      });
      server.on("/play", [&]() {
        handlePlay();
      });
      server.on("/status", [&]() {
        server.send(200, "text/plain", df_Status);
      });
      server.on("/config", [&]() {
        handleConfig();
      });
      server.on("/count", [&]() {
        uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
        server.send(200, "text/plain", String(count));
      });

      // Ergänzte Endpunkte:
      server.on("/script.js", [&]() {
        handleScriptJS();
      });
      server.on("/style.css", [&]() {
        handleStyleCSS();
      });
      server.on("/iplocal", [&]() {
        server.send(200, "text/plain", WiFi.localIP().toString());
      });
      server.on("/ssid", [&]() {
        server.send(200, "text/plain", WiFi.SSID());
      });

    }

  private:
    uint8_t modi = 1;
    uint8_t werkstattModus = 0;
    uint8_t volume = 20;
    uint8_t led;
    String df_Status = "unbekannt";

    ESP8266WebServer& server;

    class Mp3Notify;
    typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;
    DfMp3 dfmp3;

    uint16_t playDFplayer(uint16_t song) {
      dfmp3.setVolume(volume);
      dfmp3.playMp3FolderTrack(song);
      df_Status = "Spiele Song: " + String(song);
      digitalWrite(led, HIGH);
      return song;
    }

    void checkAndPlayMp3() {
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
        case 0: if (minute == 0) shouldPlay = true; break;
        case 1: if (minute == 0 || minute == 30) shouldPlay = true; break;
        case 2: if (minute % 15 == 0) shouldPlay = true; break;
      }

      if (shouldPlay) {
        playDFplayer(day);  // Tag = Tracknummer
      }
    }

    void handleRoot() {
      uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
      String html = "<!DOCTYPE html><html>";
      html += "<head>\n";
      html += "<meta charset='UTF-8'>\n";
      html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
      html += "<title>Spieluhr</title>\n";
      html += "<script src='/script.js'></script>\n";
      html += "<link rel='stylesheet' href='/style.css'>\n";

      html += "</head>\n<body>\n";
      html += "<h1>Spieluhr</h1>\n";

      html += "<div>";
      html += "<select id='songSelect'>\n";
      for (uint16_t i = 1; i <= count; i++) {
        html += "<option value='" + String(i) + "'>Song " + String(i) + "</option>\n";
      }
      html += "</select>\n";
      html += "<button onclick='playSelected()'>Play</button>\n";
      html += "</div>\n";
      html += "<div id='playResult'></div>\n";
      html += "<table border='1' style='margin: 20px auto; font-size: 14pt;'>\n";
      html += "<tr><td class='name'>Uhrzeit</td><td class='value' id='uhrzeit'>Lade Uhrzeit...</td></tr>\n";
      html += "<tr><td class='name'>Datum</td><td class='value' id='datum'>Lade Datum...</td></tr>\n";
      html += "<tr><td class='name'>SSID</td><td class='value' id='ssid'>Lade SSID...</td></tr>\n";
      html += "<tr><td class='name'>IP-Adresse</td><td class='value' id='iplocal'>Lade IP Ardesse...</td></tr>\n";
      html += "<tr><td class='name'>Anzahl Songs</td><td class='value' id='count'>Lade Songs...</td></tr>\n";
      html += "<tr><td class='name'>status</td><td class='value' id='status'>Lade Status...</td></tr>\n";
      html += "</table>\n";
    
      html += "<form action='/config' method='get'>\n";
      html += "<h3>MODI:</h3>\n";
      html += String("<input type='radio' name='modi' value='0'") + (modi == 0 ? " checked" : "") + "> 1x Std.<br>\n";
      html += String("<input type='radio' name='modi' value='1'") + (modi == 1 ? " checked" : "") + "> 2x Std.<br>\n";
      html += String("<input type='radio' name='modi' value='2'") + (modi == 2 ? " checked" : "") + "> 4x Std.<br>\n";
    
      html += "<h3>Werkstatt-Modus:</h3>\n";
      html += String("<input type='checkbox' name='werkstatt' value='1'") + (werkstattModus == 1 ? " checked" : "") + "> Mo–Fr aktiv<br>\n";
    
      html += "<br><input type='submit' value='Speichern'>\n";
      html += "</form>\n";
    
      html += "<script>\n";
      html += "function playSelected() { ";
      html += "var song = document.getElementById('songSelect').value; ";
      html += "loadDoc('/play?song=' + song, 'playResult'); ";
      html += "} ";
      html += "\n</script>\n";
      html += "</body>\n</html>\n";
      server.send(200, "text/html", html);
    }

    void handlePlay() {
      if (server.hasArg("song")) {
        playDFplayer(server.arg("song").toInt());
      }
      server.send(200, "text/plain", "Song gestartet");
    }

    void handleConfig() {
      if (server.hasArg("modi")) {
        modi = server.arg("modi").toInt();
      }
      werkstattModus = server.hasArg("werkstatt") ? 1 : 0;

      EEPROM.begin(2);
      EEPROM.write(0, modi);
      EEPROM.write(1, werkstattModus);
      EEPROM.commit();
      EEPROM.end();

      server.sendHeader("Location", "/");
      server.send(302, "text/plain", "Konfiguration gespeichert");
    }

    void handleScriptJS() {
      String js = "function loadDoc(url, id) {\n";
      js += "  var xhttp = new XMLHttpRequest();\n";
      js += "  xhttp.onreadystatechange = function() {\n";
      js += "    if (this.readyState == 4 && this.status == 200) {\n";
      js += "      document.getElementById(id).innerHTML = this.responseText;\n";
      js += "    }\n";
      js += "  };\n";
      js += "  xhttp.open(\"GET\", url, true);\n";
      js += "  xhttp.send();\n";
      js += "}\n";

      js += "window.onload = function() {\n";
      js += "  loadDoc(\"/time\", \"uhrzeit\");\n";
      js += "  loadDoc(\"/datum\", \"datum\");\n";
      js += "  loadDoc(\"/iplocal\", \"iplocal\");\n";
      js += "  loadDoc(\"/ssid\", \"ssid\");\n";
      js += "  loadDoc(\"/count\", \"count\");\n";
      js += "};\n";

      js += "setInterval(function() {\n";
      js += "  loadDoc(\"/status\", \"status\");\n";
      js += "}, 500);\n";

      js += "setInterval(function() {\n";
      js += "  loadDoc(\"/time\", \"uhrzeit\");\n";
      js += "}, 20000);\n";

      server.send(200, "application/javascript", js);
    }

    void handleStyleCSS() {
      String css = "body { font-family: sans-serif; text-align: center; padding: 20px; }\n";
      css += "h1 { font-size: 24pt; margin-bottom: 10px; }\n";
      css += "#uhrzeit { font-size: 16pt; margin-bottom: 20px; }\n";
      css += "select, button { font-size: 16pt; margin: 10px; }\n";
      css += "table { border-collapse: collapse; width: 80%; margin: auto; }\n";
      css += "td.name { font-weight: bold; background-color: #f0f0f0; padding: 8px; }\n";
      css += "td.value { padding: 8px; }\n";
      css += "table, td { border: 1px solid #ccc; }\n";
      server.send(200, "text/css", css);
    }

    class Mp3Notify {
      public:
        static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
          digitalWrite(D4, LOW);  // LED aus
        }
        static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {}
        static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
        static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
        static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {}
    };
};
