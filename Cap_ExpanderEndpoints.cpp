/*** Cap_ExpanderEndpoints.cpp */
#include "Spieluhr.h"

void Spieluhr::setupExpanderEndpoints() {
  server.on("/getoutputs", HTTP_GET, [&]() {
    uint8_t state = pcf8574->readInputs();  // oder expander->getOutputPin(i) für jeden
    String json = "{";
    for (int i = 0; i < 4; i++) {
      json += "\"P" + String(i) + "\":" + String(pcf8574->getOutputPin(i) ? "true" : "false");
      if (i < 3) json += ",";
    }
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/setoutput", HTTP_GET, [&]() {
    if (!server.hasArg("pin")) {
      server.send(400, "text/plain", "Fehlende Parameter");
      return;
    }
    uint8_t pin = server.arg("pin").toInt();

    pcf8574->toggleOutputPin(pin);
    server.send(200, "text/plain", "Pin " + String(pin));
    /*
    if (state) {
      // Auto-Off nach 30 Sekunden
      uint32_t timeout = millis() + 30000;
      scheduler->addOneShotTask(timeout, [pin]() {
        pcf8574->setOutputPin(pin, false);
      });
    }*/
  });
}