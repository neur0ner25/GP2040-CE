#include "drivermanager.h"
#include "drivers/switch2pro/Switch2ProDriver.h"
#include "usbhostmanager.h"

void DriverManager::setup(InputMode mode) {
    driver = new Switch2ProDriver();
    driver->initialize();
    inputMode = INPUT_MODE_SWITCH2_PRO;
}
