/*** Style.cpp: */
#include "Cap_WebServer.h"

void cap_Webserver::handleStyleCSS() {
  String css = "";

  css += "body {";
  css += "  font-family: sans-serif;";
  css += "  text-align: center;";
  css += "  padding: 20px;";
  css += "}\n";

  css += "h1 {  ";
  css += "  font-size: 24pt;";
  css += "  margin-bottom: 5px;";
  css += "}\n";

  /***********************************/
  css += ".tabContent {\n";
  css += "  display: none;\n";
  css += "  margin-top: 20px;\n";
  css += "}\n";

  css += ".tabContent.active {\n";
  css += "  display: block;\n";
  css += "}\n";

  /***********************************/
  //css += "#volumeSlider::-webkit-slider-thumb, #volumeSlider::-moz-range-thumb, #volumeSlider::-range-thumb {\n";
  css += "#volumeSlider::-webkit-range-thumb, #volumeSlider::-moz-range-thumb {\n";
  css += "  width: 12px;\n";
  css += "  height: 20px;\n";
  css += "  border: none;\n";
  css += "  border-radius: 2px;\n";
  css += "  cursor: pointer;\n";
  css += "}\n";

  //css += "#volumeSlider::-webkit-slider-runnable-track, #volumeSlider::-moz-range-track, #volumeSlider::-range-track {\n";
  css += "#volumeSlider::-webkit-range-track, #volumeSlider::-moz-range-track {\n";
  css += "  height: 8px;\n";
  css += "  border-radius: 3px;\n";
  css += "}\n";

  /***********************************/
  css += ".audioBar {\n";
  css += "  display: flex;\n";
  css += "  justify-content: center;\n";
  css += "  align-items: center;\n";
  css += "  gap: 10px;\n";
  css += "  margin: 20px auto;\n";
  css += "  padding: 10px;\n";
  css += "  border-radius: 8px;\n";
  css += "}\n";

  css += ".audioBar select {\n";
  css += "  font-size: 14pt;\n";
  css += "  padding: 5px;\n";
  css += "  border-radius: 4px;\n";
  css += "  border: none;\n";
  css += "}\n";

  css += ".btn {\n";
  css += "  font-size: 14pt;\n";
  css += "  padding: 8px 16px;\n";
  css += "  border-radius: 4px;\n";
  css += "  cursor: pointer;\n";
  css += "  user-select: none;\n";
  css += "  transition: background 0.3s;\n";
  css += "}\n";

  css += ".btn:hover {\n";
  css += "}\n";  // Farbe kommt aus thema.css

  /***********************************/
  css += "#cssArea {\n";
  css += "  width: 90%;\n";
  css += "  height: 300px;\n";
  css += "  font-family: monospace;\n";
  css += "  background: #000000;\n";
  css += "  color: #00ff00;\n";
  css += "  border: 1px solid #4caf50;\n";
  css += "  padding: 10px;\n";
  css += "  resize: vertical;\n";
  css += "  margin: 10px auto;\n";
  css += "  display: block;\n";
  css += "}\n";
  /***********************************/
  server.send(200, "text/css", css);
}
