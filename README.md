# 🎶 ESP8266 Spieluhr mit DFPlayer Mini
Dieses Projekt ist eine vernetzte, modular aufgebaute MP3-Spieluhr auf Basis eines ESP8266 und DFPlayer Mini. Sie spielt Musik zu festen Zeiten, kann über Touchsensoren oder ein Webinterface gesteuert werden und unterstützt OTA-Updates, WLAN-Konfiguration sowie ein Live-CSS-Editor für die Oberfläche.

## 🧰 Funktionen

- ⏰ Zeitgesteuerte Wiedergabe (z. B. jede Stunde)
- 🎵 Zufällige Songauswahl per Touchsensor oder Webinterface
- 🌐 Webinterface mit Tabs für Infos, Audio, Outputs, Konfiguration und CSS-Editor
- 🎛️ Steuerung von MOSFETs über PCF8574-Ausgänge
- 🖱️ Touchsensor-Eingänge über PCF8574 (z. B. TTP223)
- 🎨 Live-CSS-Editor zur Anpassung des Designs im Browser
- 📡 Dual-WLAN-Modus (Station + Access Point mit Captive Portal)
- 🔄 OTA-Updates über das Netzwerk
- 🧠 Konfigurationsdateien im LittleFS (JSON-basiert)

## 🛠️ Hardware
- Micro SD-Karte (2-32 GB)
- ESP8266 (z. B. D1 Mini)
- DFPlayer Mini + SD-Karte mit MP3-Dateien
- PCF8574 I²C-Portexpander
-  4x TTP223 Touchsensoren
-  4x N-Ch. MOSFETs

---
## 🛠️ SD-Kartenstruktur
Alle MP3-Dateien müssen im Verzeichnis ~/mp3/ auf der SD-Karte liegen.
Die karte muß Fat16 o. Fat32 formatiert sein.
Die Dateinamen müssen im 4.3-Format benannt sein: vierstellige Nummer + .mp3 (z. B. 0001.mp3, 0032.mp3).
/mp3/ 
├── 0001.mp3 
├── 0002.mp3 
├── 0004.mp3 
├── ...
├── 0030.mp3 
├── 0032.mp3 
├── 0033.mp3 
└── ...

---
## 🌐 Webinterface

Das Webinterface ist über IP oder Hostname erreichbar und bietet:

- **Player-Tab**: Songauswahl, Play/Stop/Random, Lautstärkeregler
- **Outputs-Tab**: Steuerung der PCF8574-Ausgänge (P0–P3)
- **Konfig-Tab**: Werkstattmodus, Wiedergabeintervall, Favicon-Upload
- **Editor-Tab**: Live-CSS-Editor mit Vorschau und Speichern
- **Info-Tab**: Uhrzeit, Datum, SSID, IP-Adresse, Songanzahl, Status
## 🚀 Installation & Nutzung

1. SD-Karte vorbereiten (`/mp3/` mit Dateien im Format `0001.mp3`)
2. Firmware flashen (z. B. über PlatformIO oder Arduino IDE)
3. Gerät starten → verbindet sich mit WLAN oder startet Access Point
4. Webinterface aufrufen → z. B. `http://spieluhr-1.local` oder IP-Adresse
5. Konfiguration vornehmen, CSS anpassen, Musik genießen 🎶

---
## 📬 Feedback & Erweiterungsideen

- Unterstützung für Favoriten oder Playlists
- Zeitfenster für Wiedergabe (z. B. nur zwischen 8–18 Uhr)
- MQTT-Integration für Smart Home
- Web-Upload für MP3-Dateien

---
Made with ❤️ by Carsten

