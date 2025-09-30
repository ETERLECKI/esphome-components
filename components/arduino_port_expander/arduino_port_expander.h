#ifndef ARDUINO_PORT_EXPANDER_H
#define ARDUINO_PORT_EXPANDER_H

#include "esphome.h"
#include "i2c_device.h"

#define ARDUINO_MAX_PIN 69  // Cambiado para Mega: pines desde 0 hasta 69 (54–69 como A0–A15)

class ArduinoPortExpander : public Component, public I2CDevice {
 public:
  ArduinoPortExpander(I2CComponent *parent, uint8_t address, bool vref_default = false)
    : I2CDevice(parent, address) {
    this->vref_default_ = vref_default;
  }
  void setup() override;
  void loop() override;

  // API para configurar pines:
  void pinModeInput(uint8_t pin);
  void pinModeInputPullup(uint8_t pin);
  void pinModeOutput(uint8_t pin);
  bool digitalReadPin(uint8_t pin);
  void digitalWritePin(uint8_t pin, bool value);
  int analogReadPin(uint8_t pin);

 protected:
  uint32_t configure_timeout_{0};
  bool vref_default_;
};

#endif  // ARDUINO_PORT_EXPANDER_H
