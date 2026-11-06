#include "drivermanager.h"
#include "drivers/switch2pro/Switch2ProDriver.h"

void DriverManager::setup(InputMode mode) {
    // ProBridge-S2 dedicated output driver: Nintendo Switch 2 Pro Controller
    driver = new Switch2ProDriver();
    driver->initialize();
    inputMode = INPUT_MODE_SWITCH2_PRO;
}

