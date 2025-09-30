#include "arduino_port_expander.h"
#include "esphome/core/log.h"

namespace esphome {

static const char *const TAG = "arduino_port_expander";

// --- Comandos que usará el Arduino Mega ---
enum ArduinoPortExpanderCommand : uint8_t {
  CMD_PIN_MODE       = 0x01,
  CMD_DIGITAL_WRITE  = 0x02,
  CMD_DIGITAL_READ   = 0x03,
  CMD_ANALOG_READ    = 0x04,
};

void ArduinoPortExpander::setup() {
  ESP_LOGI(TAG, "Inicializando ArduinoPortExpander en direccion 0x%02X", this->address_);
  this->configure_timeout_ = millis() + 5000;
}

void ArduinoPortExpander::loop() {
  // Acá podrías enviar un “ping” periódico si querés
}

// --- Implementaciones de las APIs de pin ---

void ArduinoPortExpander::pinModeInput(uint8_t pin) {
  std::vector<uint8_t> data = {CMD_PIN_MODE, pin, 0x00};  // 0x00 = INPUT
  this->parent_->write(this->address_, data);
}

void ArduinoPortExpander::pinModeInputPullup(uint8_t pin) {
  std::vector<uint8_t> data = {CMD_PIN_MODE, pin, 0x02};  // 0x02 = INPUT_PULLUP
  this->parent_->write(this->address_, data);
}

void ArduinoPortExpander::pinModeOutput(uint8_t pin) {
  std::vector<uint8_t> data = {CMD_PIN_MODE, pin, 0x01};  // 0x01 = OUTPUT
  this->parent_->write(this->address_, data);
}

bool ArduinoPortExpander::digitalReadPin(uint8_t pin) {
  uint8_t value = 0;
  std::vector<uint8_t> cmd = {CMD_DIGITAL_READ, pin};
  if (!this->parent_->write_read(this->address_, cmd, &value, 1)) {
    ESP_LOGW(TAG, "Error leyendo pin digital %d", pin);
    return false;
  }
  return value != 0;
}

void ArduinoPortExpander::digitalWritePin(uint8_t pin, bool value) {
  std::vector<uint8_t> data = {CMD_DIGITAL_WRITE, pin, static_cast<uint8_t>(value)};
  if (!this->parent_->write(this->address_, data)) {
    ESP_LOGW(TAG, "Error escribiendo pin digital %d", pin);
  }
}

int ArduinoPortExpander::analogReadPin(uint8_t pin) {
  // Para Mega: A0–A15 son 54–69
  if (pin < 54 || pin > 69) {
    ESP_LOGW(TAG, "Pin %d no es analógico válido en Mega", pin);
    return 0;
  }
  uint8_t raw[2] = {0, 0};
  std::vector<uint8_t> cmd = {CMD_ANALOG_READ, pin};
  if (!this->parent_->write_read(this->address_, cmd, raw, 2)) {
    ESP_LOGW(TAG, "Error leyendo pin analógico %d", pin);
    return 0;
  }
  int value = (raw[0] << 8) | raw[1];
  return value;
}

}  // namespace esphome
