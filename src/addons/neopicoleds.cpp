#include "addons/neopicoleds.h"
#include "BoardConfig.h"
#include "storagemanager.h"
#include "drivermanager.h"
#include "helper.h"
#include "pico/stdlib.h"

bool NeoPicoLEDAddon::available() {
    return isValidPin(BOARD_LEDS_PIN);
}

void NeoPicoLEDAddon::setup() {
    if (!isValidPin(BOARD_LEDS_PIN)) return;

    // Waveshare RP2040-Zero onboard WS2812 RGB LED on GP16
    neopico.Setup(BOARD_LEDS_PIN, 1, LED_FORMAT_GRB, pio1, 0);

    // Static red status color: G=0, R=5 (dim), B=0 in GRB format
    uint32_t grbRed = (5u << 8);
    neopico.SetFrame(&grbRed);
    neopico.Show();
    isSetup = true;
}

void NeoPicoLEDAddon::process() {
    // Single onboard status LED remains statically lit
}
