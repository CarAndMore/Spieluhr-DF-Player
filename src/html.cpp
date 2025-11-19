/*** html.cpp: */
#include "Cap_WebServer.h"

void cap_Webserver::handleRoot() {
  /***********************************/
  String title = "Spieluhr 5";
  String tabs[][2] = {
    { "Infos", "infos" },   // Info-Tabelle
    { "Player", "audio" },  // Lautstärke & AudioBar
    { "Outputs", "outputs" }, // IO Controller
    { "Konfig", "config" },    // IconUploader & Scheduler
    { "Editor", "cssEditor" }  // Live-CSS-Editor
  };
  /***********************************/
  String html = "<!DOCTYPE html><html>";
  html += "<head>\n";
  html += myHeader(title);
  html += "</head>\n<body>\n";
  /***********************************/
  html += "  <header>\n";
  html += "    <h1>Spieluhr</h1>\n";
  /***********************************/
  html += "    <nav class='tabs'>\n";
  for (int i = 0; i < sizeof(tabs) / sizeof(tabs[0]); i++) {
    html += "    <button class='btn' onclick='showTab(\"" + String(tabs[i][1]) + "\")'>" + tabs[i][0] + "</button>\n";
  }
  html += "    </nav>\n";
  html += "  </header>\n";
  /***********************************/
  html += "  <div class='tabContent' id='infos'></div>\n";
  html += "  <div class='tabContent' id='audio'>\n" + myVolume() + myAudioBar() + "</div>\n";
  html += "  <div class='tabContent' id='outputs'>\n" + myOutputControl() + "</div>\n";
  html += "  <div class='tabContent' id='config'>\n" + myFaviconUploader() + "<div id='json_config'></div>\n</div>\n";
  html += "  <div class='tabContent' id='cssEditor'>\n" + myEditor() + "</div>\n";
  /***********************************/
  
  html += "<footer>\n";
  html += "  Status: <span id='mp3status'>Lade Status...</span>\n";
  html += "  <a href='/system8266/formatC'>format! Warnung</a>\n";
  html += "</footer>\n";
  /***********************************/
  html += "</body>\n";
  html += "</html>\n";
  /***********************************/
  server.send(200, "text/html", html);
}

String cap_Webserver::myHeader(String title) {
  String html = "";
  html += "  <meta charset='UTF-8'>\n";
  html += "  <meta http-equiv='X-UA-Compatible' content='IE=edge'>\n";
  html += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  html += "  <meta name='description' content='Interaktive Spieluhr mit Weboberfläche, Audio, Touch und LED-Steuerung'>\n";
  html += "  <meta name='author' content='CarAndMore'>\n";
  html += "  <meta name='theme-color' content='#f33'>\n";  // Optional: für mobile Browser
  html += "  <title>" + title + "</title>\n";
  html += "  <link rel='icon' type='image/png' href='/favicon.png'>\n";
  html += "  <link rel='stylesheet' href='/style.css'>\n";
  html += "  <link rel='stylesheet' href='/thema.css'>\n";
  html += "  <script src='/script.js' defer></script>\n";
  return html;
}

String cap_Webserver::myFaviconUploader() {
  String html = "";
  html += "  <h2>Favicon hochladen</h2>\n";
  html += "  <img id='faviconPreview' src='/favicon.png' alt='Aktuelles Favicon' style='width:48px;height:48px;margin-bottom:10px;'><br>\n";
  html += "  <input type='file' id='faviconUpload' accept='.png'><br><br>\n";
  html += "  <button class='btn' onclick='uploadFavicon()'>Hochladen</button>\n";
  return html;
}

String cap_Webserver::myEditor() {
  String html = "";
  html += "  <h2>CSS Editor</h2>\n";
  html += "  <textarea id='cssArea'></textarea><br>\n";
  html += "  <button class='btn' onclick='saveCSS()'>Speichern</button>\n";
  html += "  <button class='btn' onclick='reloadCSS()'>Neuladen</button>\n";
  return html;
}

String cap_Webserver::myAudioBar() {
  String html = "";
  html += "  <div id='audioBar'>\n";
  html += "    <select id='trackSelect'>\n";
  html += "      <option value='1'>Track 1</option>\n";  // Dynamisch per JS ersetzt
  html += "    </select>\n";
  html += "    <button class='btn' onclick='playSelected()'>▶️</button>\n";
  html += "    <button class='btn' onclick='playRandom()'>🔀</button>\n";
  html += "    <button class='btn' onclick='stopPlayback()'>⏹️</button>\n";
  html += "  </div>\n";
  return html;
}

String cap_Webserver::myVolume() {
  String html = "";
  html += "  <fieldset><legend>Lautstärke:</legend>\n";
  html += "    <input type='range' id='volumeSlider' min='0' max='30' step='1' oninput='updateVolume(this.value)'>\n";
  html += "  </fieldset><br>\n";

  return html;
}

String cap_Webserver::myOutputControl() {
  String html = "<h2>Ausgänge</h2><div id='outputControls'>\n";
  for (int i = 0; i < 4; i++) {
    html += "  <button class='btn' onclick='toggleOutput(" + String(i) + ")'>P" + String(i) + "</button> ";
    html += "<span id='stateP" + String(i) + "'>?</span><br>\n";
  }
  html += "</div>\n";
  return html;
}

void cap_Webserver::handleInfo() {
  String html = "";
  html += "  <table id='infoTable'>\n";
  html += "    <tr><th colspan='2'>Systemzeit</th></tr>\n";
  html += "    <tr><td class='name'>Uhrzeit</td><td class='value' id='uhrzeit'>Lade Uhrzeit...</td></tr>\n";
  html += "    <tr><td class='name'>Datum</td><td class='value' id='datum'>Lade Datum...</td></tr>\n";

  html += "    <tr><th colspan='2'>WiFi</th></tr>\n";
  html += "    <tr><td class='name'>SSID</td><td class='value' id='ssid'>Lade SSID...</td></tr>\n";
  html += "    <tr><td class='name'>IP-Adresse</td><td class='value' id='iplocal'>Lade IP-Adresse...</td></tr>\n";

  html += "    <tr><th colspan='2'>Internes Filesystem</th></tr>\n";
  html += "    <tr><td class='name'>Speicher</td><td class='value'><span id='freespace'>?</span> frei</td></tr>\n";
  html += "    <tr><td class='name'>Dateien</td><td class='value' id='filecount'>?</td></tr>\n";

  html += "    <tr><th colspan='2'>Player</th></tr>\n";
  html += "    <tr><td class='name'>Anzahl Songs</td><td class='value' id='count'>Lade Songs...</td></tr>\n";
  html += "  </table>\n";
  server.send(200, "text/html", html);
}
