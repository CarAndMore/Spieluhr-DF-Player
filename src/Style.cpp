/*** Style.cpp: */
#include "Cap_WebServer.h"

void cap_Webserver::handleStyleCSS() {
  String css = "";

  css += "body {\n";
  css += "  font-family: sans-serif;\n";
  css += "  text-align: center;\n";
  css += "  padding: 10px;\n";
  css += "  padding-bottom: 45px; /* etwas mehr als Footer-Höhe */\n";
  css += "}\n\n";
  /***********************************/
  css += "h1 {\n";
  css += "  font-size: 24pt;\n";
  css += "  margin: 0;\n";
  css += "  margin-bottom: 5px;\n";
  css += "}\n\n";
  /***********************************/
  css += "a {\n";
  css += "  text-decoration: none;\n";
  css += "  font-weight: bold;\n";
  css += "}\n\n";

  css += "a:hover {\n";
  css += "  text-decoration: underline;\n";
  css += "}\n\n";
  /***********************************/
  css += "footer {\n";
  css += "  position: fixed;\n";
  css += "  bottom: 0;\n";
  css += "  left: 0;\n";
  css += "  height: 50px;\n";
  css += "  width: 100%;\n";
  css += "  font-size: 16pt;\n";
  css += "  text-align: center;\n";
  css += "  line-height: 50px; /* vertikal zentriert */\n";
  css += "  z-index: 100;\n";
  css += "  border-top: 2px solid\n";
  css += "}\n\n";
  /***********************************/
  css += "#infoTable {\n";
  css += "  margin: 20px auto;\n";
  css += "  border-collapse: collapse;\n";
  css += "  width: 320px;\n";
  css += "}\n\n";

  css += "#infoTable th {\n";
  css += "  font-size: 16pt;\n";
  css += "  text-align: left;\n";
  css += "  padding: 10px;\n";
  css += "  border-bottom: 2px solid;\n";
  css += "}\n\n";

  css += "#infoTable td {\n";
  css += "  font-size: 12pt;\n";
  css += "  padding: 8px;\n";
  css += "  border-bottom: 1px solid;\n";
  css += "}\n\n";

  css += "#infoTable td.name {\n";
  css += "  font-weight: bold;\n";
  css += "  text-align: right;\n";
  css += "  width: 40%;\n";
  css += "  border-right: 1px solid;\n";
  css += "}\n\n";

  css += "#infoTable td.value {\n";
  css += "  text-align: left;\n";
  css += "  width: 60%;\n";
  css += "}\n\n";

  /***********************************/
  css += ".tabContent {\n";
  css += "  display: none;\n";
  css += "  margin-top: 20px;\n";
  css += "}\n\n";

  css += ".tabContent.active {\n";
  css += "  display: block;\n";
  css += "}\n\n";

  /***********************************/
  //css += "#volumeSlider::-webkit-slider-thumb, #volumeSlider::-moz-range-thumb, #volumeSlider::-range-thumb {\n";
  css += "#volumeSlider::-webkit-range-thumb, #volumeSlider::-moz-range-thumb {\n";
  css += "  width: 12px;\n";
  css += "  height: 20px;\n";
  css += "  border: none;\n";
  css += "  border-radius: 2px;\n";
  css += "  cursor: pointer;\n";
  css += "}\n\n";

  //css += "#volumeSlider::-webkit-slider-runnable-track, #volumeSlider::-moz-range-track, #volumeSlider::-range-track {\n";
  css += "#volumeSlider::-webkit-range-track, #volumeSlider::-moz-range-track {\n";
  css += "  height: 8px;\n";
  css += "  border-radius: 3px;\n";
  css += "}\n\n";
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
  /***********************************/
  css += "select {\n";
  css += "  font-size: 14pt;\n";
  css += "  padding: 8px 16px;\n";
  css += "  margin-bottom: 5px;\n";
  css += "  border: 2px solid;\n";
  css += "  border-radius: 4px;\n";
  css += "  border-top-left-radius: 24px;\n";
  css += "  border-bottom-left-radius: 24px;\n";
  css += "  cursor: pointer;\n";
  css += "  user-select: none;\n";
  css += "  transition: background 0.3s;\n";
  css += "}\n";
  /***********************************/
  css += "input[type='number'] {\n";
  css += "  font-size: 14pt;\n";
  css += "  padding: 8px 16px;\n";
  css += "  border: 2px solid;\n";
  css += "  border-radius: 8px;\n";
  css += "  transition: background 0.3s;\n";
  css += "  appearance: textfield;\n";  // entfernt Pfeile in manchen Browsern
  css += "}\n";
  /***********************************/

  css += ".checkboxWrap {\n";
  css += "  display: inline-flex;\n";
  css += "  align-items: center;\n";
  css += "  gap: 8px;\n";
  css += "  font-size: 12pt;\n";
  css += "  cursor: pointer;\n";
  css += "}\n";

  css += ".checkboxWrap input[type='checkbox'] {\n";
  css += "  display: none;\n";
  css += "}\n";

  css += ".checkboxCustom {\n";
  css += "  width: 28px;\n";
  css += "  height: 28px;\n";
  css += "  border: 3px solid\n";
  css += "  border-radius: 4px;\n";
  css += "  transition: background 0.3s;\n";
  css += "  position: relative;\n";
  css += "}\n";

  css += ".checkboxWrap input[type='checkbox']:checked + .checkboxCustom::after {\n";
  css += "  content: '';\n";
  css += "  position: absolute;\n";
  css += "  top: 5px;\n";
  css += "  left: 9px;\n";
  css += "  width: 6px;\n";
  css += "  height: 14px;\n";
  css += "  border: solid #ffffff\n";
  css += "  border-width: 0 2px 2px 0;\n";
  css += "  transform: rotate(45deg);\n";
  css += "}\n";
  /***********************************/
  css += "input[type='file'] {\n";
  css += "  font-size: 12pt;\n";
  css += "  cursor: pointer;\n";
  css += "  user-select: none;\n";
  css += "}\n";

  css += "input[type='file']::file-selector-button {\n";
  css += "  padding: 8px 16px;\n";
  css += "  border: 4px solid:\n";
  css += "  border-radius: 24px;\n";
  css += "  transition: background 0.3s;\n";
  css += "}\n";
  /***********************************/
  css += ".btn {\n";
  css += "  font-size: 14pt;\n";
  css += "  margin-bottom: 15px;\n";
  css += "  padding: 8px 16px;\n";
  css += "  border: 4px solid;\n";
  css += "  border-radius: 24px;\n";
  css += "  cursor: pointer;\n";
  css += "  user-select: none;\n";
  css += "  transition: background 0.3s;\n";
  css += "  margin: 0;\n";
  css += "}\n";
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
  css += "}\n\n";
  /***********************************/
  server.send(200, "text/css", css);
}
