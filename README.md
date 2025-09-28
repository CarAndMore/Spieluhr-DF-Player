# Spieluhr-DF-Player

# 🎶 ESP8266 Spieluhr mit DFPlayer Mini

Dieses Projekt ist eine vernetzte, zeitgesteuerte MP3-Spieluhr auf Basis eines ESP8266 und DFPlayer Mini. Sie spielt Musik zu festen Zeiten, kann über einen Taster oder ein Webinterface gesteuert werden und unterstützt OTA-Updates sowie WLAN-Konfiguration.

## 🧰 Funktionen

- ⏰ Zeitgesteuerte Wiedergabe (1x, 2x oder 4x pro Stunde)
- 🎵 Zufällige Songauswahl per Taster
- 🌐 Webinterface zur Steuerung und Konfiguration
- 💾 EEPROM-Speicherung von Einstellungen
- 📡 Dual-WLAN-Modus (Station + Access Point)
- 🔄 OTA-Updates über das Netzwerk

## 🛠️ Hardware

- ESP8266 (D1 Mini)
- DFPlayer Mini + SD-Karte mit MP3-Dateien
- LED (Statusanzeige)
- Taster (zufällige Wiedergabe)
- MOSFET zur LED-Steuerung

## 🛠️ SDcartenaufbau
Alle MP3-Dateien müssen im Verzeichnis ~/mp3/ auf der SD-Karte liegen.

Die Dateinamen müssen im 4.3-Format benannt sein: vierstellige Nummer + .mp3 (z. B. 0001.mp3, 0032.mp3).

~/mp3/

  ├── 0001.mp3
  
  ├── 0002.mp3
  
  ├── 0004.mp3
  
  ├── 0030.mp3
  
  ├── 0032.mp3
  
  ├── 0033.mp3
  
  ├── ...
  
##  🌐 Webinterface

Das Webinterface bietet:

Songauswahl und Wiedergabe

Anzeige von Uhrzeit, Datum, SSID, IP-Adresse, Songanzahl

Konfiguration von Wiedergabemodus und Werkstatt-Modus

##  ⚙️ Konfigurierbare Modi

Modus 0: Wiedergabe jede volle Stunde

Modus 1: Wiedergabe jede volle und halbe Stunde

Modus 2: Wiedergabe alle 15 Minuten

Werkstatt-Modus: Aktiv nur Mo–Fr (optional)

