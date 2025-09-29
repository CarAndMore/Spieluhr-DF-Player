
/* Version 0.23 */

#include "SpieluhrDFPlayer.h"


/**
 * Registriert alle HTTP-Endpunkte für die Spieluhr-Weboberfläche im ESP8266-Webserver.
 * 
 * Die Endpunkte ermöglichen:
 * Diese Funktion sollte im `begin()`-Setup der Klasse aufgerufen werden, um die Webschnittstelle zu aktivieren.
 */
void SpieluhrDFPlayer::setupWebEndpoints() {

  server.on("/generate_204", [&]() {
    handleRoot();
  });
  server.on("/hotspot-detect.html", [&]() {
    handleRoot();
  });
  server.on("/redirect", [&]() {
    handleRoot();
  });

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
  server.on("/time", [&]() {
    handleTimeNTP();
  });
  server.on("/datum", [&]() {
    handleDatumNTP();
  });
  server.on("/count", [&]() {
    uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    server.send(200, "text/plain", String(count));
  });

  server.on("/setvolume", [&]() {
    if (server.hasArg("value")) {
      uint8_t newVol = server.arg("value").toInt();
      if (newVol >= 2 && newVol <= 30) {
        volume = newVol;
        dfmp3.setVolume(volume);
        server.send(200, "text/plain", String(volume));
        df_Status = "Lautstärke gesetzt: " + String(volume);
      } else {
        server.send(400, "text/plain", "Ungültiger Wert");
      }
    } else {
      server.send(400, "text/plain", "Kein Wert übergeben");
    }
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


/**
 * Generiert und sendet die index.html-Seite der Spieluhr an den Client.
 * 
 * Diese Seite enthält:
 * - Eine Dropdown-Liste zur Auswahl eines Songs
 * - Einen Play-Button zur Wiedergabe des ausgewählten Songs
 * - Eine Status-Tabelle mit Uhrzeit, Datum, SSID, IP-Adresse, Songanzahl und Playerstatus
 * - Ein Formular zur Konfiguration des Wiedergabemodus und Werkstatt-Modus
 * 
 * Die Seite bindet zusätzlich ein externes JavaScript und CSS ein und definiert eine
 * einfache JavaScript-Funktion zur Song-Wiedergabe über einen AJAX-Request.
 */
void SpieluhrDFPlayer::handleRoot() {
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

  html += "<label for='volumeSlider'>Lautstärke: <span id='volValue'>--</span></label><br>";

  html += "<input type='range' id='volumeSlider' min='2' max='30' value='" + String(volume) + "' />";

  html += "<button onclick='setVolume()'>Setzen</button>";

  html += "<form action='/config' method='get'>\n";
  html += "<h3>MODI:</h3>\n";
  html += String("<input type='radio' name='modi' value='0'") + (modi == 0 ? " checked" : "") + "> 1x Std.<br>\n";
  html += String("<input type='radio' name='modi' value='1'") + (modi == 1 ? " checked" : "") + "> 2x Std.<br>\n";
  html += String("<input type='radio' name='modi' value='2'") + (modi == 2 ? " checked" : "") + "> 4x Std.<br>\n";

  html += "<h3>Werkstatt-Modus:</h3>\n";
  html += String("<input type='checkbox' name='werkstatt' value='1'") + (werkstattModus == 1 ? " checked" : "") + "> Mo–Fr aktiv<br>\n";

  html += "<h3>Zeitfenster:</h3>\n";
  html += String("<input type='checkbox' name='timemode' value='1'") + (timeMode == 1 ? " checked" : "") + "> Nur zwischen Start- und Endzeit aktiv<br><br>\n";

  html += "Startzeit: <input type='number' name='starthh' min='0' max='23' value='" + String(startHH) + "'> : ";
  html += "<input type='number' name='startmm' min='0' max='59' value='" + String(startMM) + "'><br>\n";

  html += "Endzeit: <input type='number' name='endhh' min='0' max='23' value='" + String(endHH) + "'> : ";
  html += "<input type='number' name='endmm' min='0' max='59' value='" + String(endMM) + "'><br>\n";

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


/**
 * Behandelt den Web-Endpunkt "/play" zur Wiedergabe eines ausgewählten Songs.
 * 
 * Wenn ein Parameter "song" übergeben wurde, wird der entsprechende Track über den DFPlayer Mini abgespielt.
 * Anschließend wird eine einfache Textantwort an den Client gesendet.
 * 
 * Beispielaufruf: /play?song=5
 */
void SpieluhrDFPlayer::handlePlay() {
  if (server.hasArg("song")) {
    playDFplayer(server.arg("song").toInt());
  }
  server.send(200, "text/plain", "Song gestartet");
}

/**
 * Behandelt den Web-Endpunkt "/config" zur Konfiguration der Spieluhr-Einstellungen.
 * 
 * Liest die Parameter "modi" und "werkstatt" aus der HTTP-Anfrage und speichert sie in der EEPROM.
 * - "modi" bestimmt die Anzahl der Stundenzyklen (z. B. 1x, 2x, 4x)
 * - "werkstatt" aktiviert den Werkstatt-Modus (werktags aktiv)
 * 
 * Nach dem Speichern wird der Client per HTTP-Redirect zurück zur Hauptseite geleitet.
 * 
 * Beispielaufruf: /config?modi=2&werkstatt=1
 */
void SpieluhrDFPlayer::handleConfig() {
  if (server.hasArg("modi")) modi = server.arg("modi").toInt();
  werkstattModus = server.hasArg("werkstatt") ? 1 : 0;
  timeMode = server.hasArg("timemode") ? 1 : 0;

  startHH = server.arg("starthh").toInt();
  startMM = server.arg("startmm").toInt();
  endHH = server.arg("endhh").toInt();
  endMM = server.arg("endmm").toInt();

  EEPROM.begin(7);
  EEPROM.write(0, modi);
  EEPROM.write(1, werkstattModus);
  EEPROM.write(2, timeMode);
  EEPROM.write(3, startHH);
  EEPROM.write(4, startMM);
  EEPROM.write(5, endHH);
  EEPROM.write(6, endMM);
  EEPROM.end();

  server.sendHeader("Location", "/");
  server.send(303);
}

/**
 * Behandelt den Web-Endpunkt "/script.js" und liefert das JavaScript für die Spieluhr-Webseite.
 * 
 * Das Skript enthält:
 * - Eine Funktion `loadDoc(url, id)` zum asynchronen Nachladen von Inhalten via XMLHttpRequest
 * - Initiales Laden von Uhrzeit, Datum, IP-Adresse, SSID und Songanzahl beim Seitenaufruf
 * - Regelmäßige Aktualisierung des Playerstatus alle 500 ms
 * - Regelmäßige Aktualisierung der Uhrzeit alle 20 Sekunden
 * 
 * Dieses Skript wird beim Laden der Hauptseite eingebunden und sorgt für dynamische UI-Aktualisierung.
 */
void SpieluhrDFPlayer::handleScriptJS() {
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

  js += "function setVolume() {\n";
  js += "  const slider = document.getElementById('volumeSlider');\n";
  js += "  fetch('/setvolume?value=' + slider.value)\n";
  js += "    .then(res => res.text())\n";
  js += "    .then(txt => alert('Lautstärke gesetzt: ' + txt));\n";
  js += "}\n";

  js += "window.onload = function() {\n";
  js += "  const slider = document.getElementById('volumeSlider');\n";
  js += "  const volValue = document.getElementById('volValue');\n";
  js += "  volValue.textContent = slider.value;\n";
  js += "  slider.oninput = () => {\n";
  js += "    volValue.textContent = slider.value;\n";
  js += "  };\n";
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

/**
 * Behandelt den Web-Endpunkt "/style.css" und liefert das CSS-Stylesheet für die Spieluhr-Webseite.
 * 
 * Das Stylesheet definiert:
 * - Schriftarten und Layout für die gesamte Seite
 * - Formatierung für Überschriften, Auswahlfelder und Buttons
 * - Tabellenstil mit klarer Struktur und abgesetzten Zellen für Namen und Werte
 * 
 * Das CSS wird direkt als Text vom Server gesendet und beim Laden der Hauptseite eingebunden.
 */
void SpieluhrDFPlayer::handleStyleCSS() {
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

/**
 * Behandelt den Web-Endpunkt "/time" und liefert die aktuelle Uhrzeit im Format HH:MM:SS.
 * 
 * Die Zeit wird lokal vom System abgerufen und formatiert, wobei führende Nullen ergänzt werden.
 * Die Ausgabe erfolgt als einfacher Text (Content-Type: text/plain).
 * 
 * Diese Funktion wird regelmäßig vom JavaScript der Hauptseite aufgerufen, um die Uhrzeit aktuell zu halten.
 */
void SpieluhrDFPlayer::handleTimeNTP() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  String Uhrzeit = "";
  Uhrzeit += kleinerNull(timeinfo->tm_hour);
  Uhrzeit += ":" + kleinerNull(timeinfo->tm_min);
  Uhrzeit += ":" + kleinerNull(timeinfo->tm_sec);
  server.send(200, "text/plain", Uhrzeit);
}

/**
 * Behandelt den Web-Endpunkt "/datum" und liefert das aktuelle Datum im Format TT.MM.JJJJ.
 * 
 * Die Datumswerte werden lokal vom System abgerufen und formatiert, wobei führende Nullen ergänzt werden.
 * Die Ausgabe erfolgt als einfacher Text (Content-Type: text/plain).
 * 
 * Diese Funktion wird vom JavaScript der Hauptseite verwendet, um das Datum regelmäßig zu aktualisieren.
 */
void SpieluhrDFPlayer::handleDatumNTP() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);

  String datum = "";
  datum += kleinerNull(timeinfo->tm_mday) + ".";
  datum += kleinerNull(timeinfo->tm_mon + 1) + ".";
  datum += String(timeinfo->tm_year + 1900);

  server.send(200, "text/plain", datum);
}

/**
 * Formatiert eine Ganzzahl als zweistellige Zeichenkette mit führender Null bei Werten unter 10.
 * 
 * Diese Funktion wird verwendet zur Anzeige von Uhrzeit- und Datumswerten im Format "HH:MM:SS" oder "TT.MM.JJJJ".
 * 
 * @param value Der ganzzahlige Wert, der formatiert werden soll (z. B. Stunde, Minute, Tag).
 * @return Eine Zeichenkette mit führender Null, falls der Wert kleiner als 10 ist.
 */
String SpieluhrDFPlayer::kleinerNull(int value) {
  if (value < 10) {
    return "0" + String(value);
  } else {
    return String(value);
  }
}
/************************/
