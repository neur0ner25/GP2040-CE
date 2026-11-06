#include "peripheralmanager.h"
#include "storagemanager.h"

void PeripheralManager::initUSB() {
    const PeripheralOptions& peripheralOptions = Storage::getInstance().getPeripheralOptions();
    if (peripheralOptions.blockUSB0.enabled) {
        blockUSB0.setConfig(0, peripheralOptions.blockUSB0.dp, peripheralOptions.blockUSB0.enable5v, peripheralOptions.blockUSB0.order);
    }
}

PeripheralUSB* PeripheralManager::getUSB(uint8_t block) {
    if (block < NUM_USBS) {
        return &blockUSB0;
    }
    return nullptr;
}

bool PeripheralManager::isUSBEnabled(uint8_t block) {
    if (block < NUM_USBS) {
        return ((block == 0) ? blockUSB0.configured : false);
    }
    return false;
}
