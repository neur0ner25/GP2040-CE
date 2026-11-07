/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2024 OpenStickCommunity (gp2040-ce.info)
 */

#ifndef GP2040_H_
#define GP2040_H_

#ifndef WEB_CONFIG_HOSTNAME
#define WEB_CONFIG_HOSTNAME "gp2040-ce"
#endif

#include <map>

// GP2040 Classes
#include "gamepad.h"
#include "addonmanager.h"
#include "gpdriver.h"
#include "system.h"

#include "pico/types.h"

class GP2040 {
public:
    GP2040(){}
    ~GP2040(){}
    void setup();           // setup core0
    void run();             // loop core0

    static void requestSave(bool force = true, bool restart = false) {
        saveRequested = true;
        forceSave = force;
        if (restart) {
            rebootRequested = true;
            rebootMode = System::BootMode::DEFAULT;
        }
    }

    static void requestReboot(System::BootMode mode = System::BootMode::DEFAULT) {
        rebootRequested = true;
        rebootMode = mode;
    }

private:
    Gamepad snapshot;
    AddonManager addons;
    // GPIO debouncer
    void debounceGpioGetAll();
    Mask_t buttonGpios = 0;
    uint32_t gpioDebounceTime[NUM_BANK0_GPIOS] = {};

    struct RebootHotkeys {
        RebootHotkeys();
        void process(Gamepad* gamepad, bool configMode);

        bool active;

        absolute_time_t noButtonsPressedTimeout;
        uint16_t webConfigHotkeyMask;
        uint16_t bootselHotkeyMask;
        absolute_time_t rebootHotkeysHoldTimeout;
    };
    RebootHotkeys rebootHotkeys;

    enum class BootActionType {
        ENTER_USB_MODE,
        SET_INPUT_MODE
    };

    struct BootAction {
        BootActionType type = BootActionType::SET_INPUT_MODE;
        InputMode inputMode = INPUT_MODE_SWITCH2_PRO;
        uint32_t profileNumber = 0;
    };

    BootAction getGpioMappedBootAction();
    BootAction getButtonMappedBootAction();

    void getReinitGamepad(Gamepad * gamepad);

    // GPIO manipulation for setup and profile reinit
    void initializeStandardGpio();
    void deinitializeStandardGpio();

    void checkSaveRebootState();

    static inline bool saveRequested = false;
    static inline bool forceSave = false;
    static inline bool saveSuccessful = false;
    static inline bool rebootRequested = false;
    static inline System::BootMode rebootMode = System::BootMode::DEFAULT;
};

#endif
