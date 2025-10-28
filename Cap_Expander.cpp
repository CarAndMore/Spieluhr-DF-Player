/*** Cap_Expander.cpp: */
#include "Cap_Expander.h"

IO_Expander::IO_Expander(uint8_t i2cAddress, TwoWire& wireBus)
  : _address(i2cAddress), _wire(wireBus) {
  }

IO_Expander::IO_Expander(uint8_t i2cAddress)
  : _address(i2cAddress), _wire(Wire) {
}

bool IO_Expander::begin() {
  _wire.begin();
  _wire.beginTransmission(_address);
  return _wire.endTransmission() == 0;
}

bool IO_Expander::writeOutputs(uint8_t value) {
  _outputState = value & 0x0F;
  uint8_t mask = _outputState | 0xF0;
  _wire.beginTransmission(_address);
  _wire.write(mask);
  return _wire.endTransmission() == 0;
}

bool IO_Expander::setOutputPin(uint8_t pin, bool state) {
  if (pin > 3) return false;
  bitWrite(_outputState, pin, state);
  return writeOutputs(_outputState);
}

bool IO_Expander::toggleOutputPin(uint8_t pin) {
  if (pin > 3) return false;
  bitWrite(_outputState, pin, !bitRead(_outputState, pin));
  return writeOutputs(_outputState);
}

bool IO_Expander::getOutputPin(uint8_t pin) {
  if (pin > 3) return false;
  return bitRead(_outputState, pin);
}

uint8_t IO_Expander::readInputs() {
  _wire.requestFrom((uint8_t)_address, (uint8_t)1);
  if (_wire.available()) {
    uint8_t raw = _wire.read();
    return (raw >> 4) & 0x0F;
  }
  return 0x0F;
}

bool IO_Expander::readInputPin(uint8_t pin) {
  if (pin > 3) return false;
  return bitRead(readInputs(), pin);
}

bool IO_Expander::readDebouncedInputPin(uint8_t pin, uint16_t debounceMs) {
  if (pin > 3) return false;

  bool current = readInputPin(pin);
  DebounceState& state = _debounce[pin];

  if (current != state.lastRead) {
    state.lastChange = millis();
    state.lastRead = current;
  }

  if (millis() - state.lastChange > debounceMs) {
    state.stableState = current;
  }

  return state.stableState;
}

bool IO_Expander::isRisingEdgeDebounced(uint8_t pin, uint16_t debounceMs) {
  if (pin > 3) return false;

  bool current = readInputPin(pin);
  DebounceState& state = _debounce[pin];

  if (current != state.lastRead) {
    state.lastChange = millis();
    state.lastRead = current;
  }

  if (millis() - state.lastChange > debounceMs) {
    if (!state.stableState && current) {
      state.stableState = true;
      return true;  // Rising edge erkannt
    }
    state.stableState = current;
  }

  return false;
}
