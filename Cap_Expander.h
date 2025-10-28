/*** Cap_Expander.h: */
#pragma once
/*** Cap_Expander.h:
 *  Steuerung eines PCF8574 I²C-Portexpanders mit fester Pinzuweisung.
 * 
 *  Diese Klasse kapselt die Kommunikation mit einem PCF8574-Portexpander über I²C.
 *  Sie erlaubt die gezielte Steuerung der unteren 4 Pins (P0–P3) als digitale Ausgänge
 *  sowie das Auslesen der oberen 4 Pins (P4–P7) als digitale Eingänge.
 * 
 *  Besonderheiten:
 *  - P0–P3 werden ausschließlich als Output behandelt (für LEDs, MosFets, Relais)
 *  - P4–P7 werden ausschließlich als Input gelesen (TTP223 Touchsensoren)
 *  - Die Klasse verwaltet den Output-Zustand intern, sodass einzelne Pins gezielt
 *    gesetzt, getoggelt oder abgefragt werden können, ohne andere zu beeinflussen
 *  - Der I²C-Bus (`Wire`, `Wire1`, etc.) wird extern übergeben, ideal für ESP32, Pi Pico usw.
 * 
 *  Beispielnutzung:
 *  @code
 *  IO_Expander pcf(0x20, Wire);
 *  pcf.begin();
 *  pcf.setOutputPin(0, LOW); // P0 aktivieren
 *  bool touched = pcf.readInputPin(2); // P6 prüfen
 *  @endcode
 */
#include <Wire.h>
#include <Arduino.h>

/**
 * @class IO_Expander
 * @brief Steuerung eines PCF8574-Portexpanders über I²C mit fester Pinzuweisung.
 */
class IO_Expander {
public:
  /**
    * Erstellt ein IO_Expander-Objekt mit benutzerdefiniertem I²C-Bus.
    * 
    * @param i2cAddress I²C-Adresse des PCF8574 (z. B. 0x20)
    * @param wireBus Referenz auf den verwendeten I²C-Bus (z. B. Wire, Wire1)
    */
  IO_Expander(uint8_t i2cAddress, TwoWire& wireBus);

  /**
    * Erstellt ein IO_Expander-Objekt mit Standard-I²C-Bus (Wire).
    * 
    * @param i2cAddress I²C-Adresse des PCF8574
    */
  IO_Expander(uint8_t i2cAddress);

  /**
    * Prüft die Verbindung zum PCF8574.
    * 
    * @return true, wenn der Expander erreichbar ist; sonst false
    */
  bool begin();

  /**
    * Schreibt einen 4-Bit-Wert auf die Output-Pins P0–P3.
    * P4–P7 bleiben hochohmig (Input).
    * 
    * @param value 4-Bit-Wert für P0–P3 (nur untere 4 Bits werden verwendet)
    * @return true bei erfolgreichem Schreiben; sonst false
    */
  bool writeOutputs(uint8_t value);

  /**
    * Setzt gezielt einen einzelnen Output-Pin (P0–P3) auf HIGH oder LOW.
    * Andere Pins bleiben unverändert.
    * 
    * @param pin Pin-Index 0–3 (entspricht P0–P3)
    * @param state true = HIGH (inaktiv), false = LOW (aktiv)
    * @return true bei Erfolg; false bei ungültigem Pin
    */
  bool setOutputPin(uint8_t pin, bool state);

  /**
    * Toggelt den Zustand eines einzelnen Output-Pins (P0–P3).
    * 
    * @param pin Pin-Index 0–3 (entspricht P0–P3)
    * @return true bei Erfolg; false bei ungültigem Pin
    */
  bool toggleOutputPin(uint8_t pin);

  /**
    * Gibt den aktuellen Zustand eines Output-Pins (P0–P3) zurück.
    * 
    * @param pin Pin-Index 0–3 (entspricht P0–P3)
    * @return true = HIGH, false = LOW
    */
  bool getOutputPin(uint8_t pin);

  /**
    * Liest den Zustand aller Input-Pins P4–P7.
    * 
    * @return 4-Bit-Wert der Eingänge (Bit 0 = P4, Bit 3 = P7)
    */
  uint8_t readInputs();

  /**
    * Liest gezielt den Zustand eines einzelnen Input-Pins (P4–P7).
    * 
    * @param pin Pin-Index 0–3 (entspricht P4–P7)
    * @return true = HIGH (Touch erkannt), false = LOW (kein Touch)
    */
  bool readInputPin(uint8_t pin);

  /**
    * Liest einen entprellten Zustand eines Input-Pins (P4–P7).
    * 
    * @param pin Pin-Index 0–3 (entspricht P4–P7)
    * @param debounceMs Entprellzeit in Millisekunden (Standard: 50 ms)
    * @return true = stabil HIGH, false = stabil LOW
    */
  bool readDebouncedInputPin(uint8_t pin, uint16_t debounceMs = 50);
  bool isRisingEdgeDebounced(uint8_t pin, uint16_t debounceMs = 80);
private:
  /**
   * Interner Zustand zur Entprellung der Input-Pins P4–P7.
   */
  struct DebounceState {
    bool lastRead = false;
    bool stableState = false;
    unsigned long lastChange = 0;
  };

  DebounceState _debounce[4];  // Für P4–P7

  /**
    * I²C-Adresse des PCF8574-Expanders.
    * Wird beim Konstruktor gesetzt und für alle Busoperationen verwendet.
    */
  uint8_t _address;

  /**
    * Interner Zustand der Output-Pins P0–P3.
    * Nur die unteren 4 Bits werden verwendet.
    * Ermöglicht gezielte Pinmanipulation ohne Beeinflussung anderer Outputs.
    */
  uint8_t _outputState = 0x0F;

  /**
    * Referenz auf den verwendeten I²C-Bus (z. B. Wire, Wire1).
    * Wird beim Konstruktor übergeben, um flexible Buswahl zu ermöglichen.
    */
  TwoWire& _wire;
};
