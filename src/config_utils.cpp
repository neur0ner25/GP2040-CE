#include "config_utils.h"

#include "config.pb.h"
#include "enums.pb.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "pb_common.h"
#include "types.h"

#include "BoardConfig.h"
#include "GamepadConfig.h"
#include "version.h"
#include "addons/board_led.h"
#include "addons/bootsel_button.h"
#include "addons/neopicoleds.h"
#include "addons/gamepad_usb_host.h"

#ifndef HAS_I2C_DISPLAY
#define HAS_I2C_DISPLAY 0
#endif
#ifndef DISPLAY_I2C_BLOCK
#define DISPLAY_I2C_BLOCK 0
#endif
#ifndef DISPLAY_I2C_ADDR
#define DISPLAY_I2C_ADDR 0x3C
#endif
#ifndef I2C_SPEED
#define I2C_SPEED 400000
#endif
#ifndef BUTTON_LAYOUT
#define BUTTON_LAYOUT BUTTON_LAYOUT_STICK
#endif
#ifndef BUTTON_LAYOUT_RIGHT
#define BUTTON_LAYOUT_RIGHT BUTTON_LAYOUT_ARCADE
#endif
#ifndef DISPLAY_TURN_OFF_WHEN_SUSPENDED
#define DISPLAY_TURN_OFF_WHEN_SUSPENDED 0
#endif
#ifndef DISPLAY_INPUT_MODE
#define DISPLAY_INPUT_MODE 1
#endif
#ifndef DISPLAY_TURBO_MODE
#define DISPLAY_TURBO_MODE 1
#endif
#ifndef DISPLAY_DPAD_MODE
#define DISPLAY_DPAD_MODE 1
#endif
#ifndef DISPLAY_SOCD_MODE
#define DISPLAY_SOCD_MODE 1
#endif
#ifndef DISPLAY_MACRO_MODE
#define DISPLAY_MACRO_MODE 1
#endif
#ifndef DISPLAY_PROFILE_MODE
#define DISPLAY_PROFILE_MODE 0
#endif
#ifndef INPUT_HISTORY_ENABLED
#define INPUT_HISTORY_ENABLED 0
#endif
#ifndef INPUT_HISTORY_LENGTH
#define INPUT_HISTORY_LENGTH 21
#endif
#ifndef INPUT_HISTORY_COL
#define INPUT_HISTORY_COL 0
#endif
#ifndef INPUT_HISTORY_ROW
#define INPUT_HISTORY_ROW 7
#endif
#ifndef SPLASH_MODE
#define SPLASH_MODE 0
#endif
#ifndef SPLASH_CHOICE
#define SPLASH_CHOICE 0
#endif
#ifndef SPLASH_DURATION
#define SPLASH_DURATION 7500
#endif
#ifndef DEFAULT_SPLASH
#define DEFAULT_SPLASH 0x00
#endif
#ifndef DISPLAY_SIZE
#define DISPLAY_SIZE 0
#endif
#ifndef DISPLAY_FLIP
#define DISPLAY_FLIP 0
#endif
#ifndef DISPLAY_INVERT
#define DISPLAY_INVERT 0
#endif
#ifndef DISPLAY_SAVER_TIMEOUT
#define DISPLAY_SAVER_TIMEOUT 0
#endif
#ifndef DISPLAY_SAVER_MODE
#define DISPLAY_SAVER_MODE 0
#endif
#ifndef DISPLAY_LAYOUT_ORIENTATION
#define DISPLAY_LAYOUT_ORIENTATION 0
#endif
#ifndef DISPLAY_CONTRAST
#define DISPLAY_CONTRAST 0xFF
#endif

#ifndef I2C_PCF8575_ENABLED
#define I2C_PCF8575_ENABLED 0
#define I2C_PCF8575_BLOCK 0
#endif

#ifndef I2C_ANALOG1115_ENABLED
#define I2C_ANALOG1115_ENABLED 0
#define ANALOG1115_CHANNEL_ENABLE 0
#define ANALOG1115_INNER_DEADZONE_ENABLE 0
#define ANALOG1115_OUTER_DEADZONE_ENABLE 0
#define ANALOG1115_LSTICK_DEADZONE_ENABLE 0
#define ANALOG1115_RSTICK_DEADZONE_ENABLE 0
#define ANALOG1115_LSTICK_DEADZONE 0
#define ANALOG1115_RSTICK_DEADZONE 0
#define ANALOG1115_CHANNEL_INNER_DEADZONE 0
#define ANALOG1115_CHANNEL_OUTER_DEADZONE 0
#define ANALOG1115_INVERT 0
#define ANALOG1115_AUTOCALIBRATE 0
#define ANALOG1115_LX_CHANNEL 0
#define ANALOG1115_LY_CHANNEL 1
#define ANALOG1115_RX_CHANNEL 2
#define ANALOG1115_RY_CHANNEL 3
#endif

#ifndef I2C_ANALOG1219_ENABLED
#define I2C_ANALOG1219_ENABLED 0
#define I2C_ANALOG1219_BLOCK 0
#define I2C_ANALOG1219_ADDRESS 0x40
#define I2C_ANALOG1219_SPEED 400000
#endif

#ifndef SPI_ANALOG1256_ENABLED
#define SPI_ANALOG1256_ENABLED 0
#define SPI_ANALOG1256_BLOCK 0
#define SPI_ANALOG1256_CS_PIN -1
#define SPI_ANALOG1256_DRDY_PIN -1
#define ADS1256_MAX_3V 3.3f
#endif

#ifndef WII_EXTENSION_ENABLED
#define WII_EXTENSION_ENABLED 0
#define WII_EXTENSION_I2C_BLOCK 0
#define WII_EXTENSION_I2C_SPEED 400000
#endif

#ifndef I2C0_ENABLED
#define I2C0_ENABLED 0
#endif
#ifndef I2C0_PIN_SDA
#define I2C0_PIN_SDA -1
#endif
#ifndef I2C0_PIN_SCL
#define I2C0_PIN_SCL -1
#endif
#ifndef I2C0_SPEED
#define I2C0_SPEED 400000
#endif

#ifndef I2C1_ENABLED
#define I2C1_ENABLED 0
#endif
#ifndef I2C1_PIN_SDA
#define I2C1_PIN_SDA -1
#endif
#ifndef I2C1_PIN_SCL
#define I2C1_PIN_SCL -1
#endif
#ifndef I2C1_SPEED
#define I2C1_SPEED 400000
#endif

#ifndef SPI0_ENABLED
#define SPI0_ENABLED 0
#endif
#ifndef SPI0_PIN_RX
#define SPI0_PIN_RX -1
#endif
#ifndef SPI0_PIN_CS
#define SPI0_PIN_CS -1
#endif
#ifndef SPI0_PIN_SCK
#define SPI0_PIN_SCK -1
#endif
#ifndef SPI0_PIN_TX
#define SPI0_PIN_TX -1
#endif

#ifndef SPI1_ENABLED
#define SPI1_ENABLED 0
#endif
#ifndef SPI1_PIN_RX
#define SPI1_PIN_RX -1
#endif
#ifndef SPI1_PIN_CS
#define SPI1_PIN_CS -1
#endif
#ifndef SPI1_PIN_SCK
#define SPI1_PIN_SCK -1
#endif
#ifndef SPI1_PIN_TX
#define SPI1_PIN_TX -1
#endif

#ifndef SNES_PAD_ENABLED
#define SNES_PAD_ENABLED 0
#define SNES_PAD_CLOCK_PIN -1
#define SNES_PAD_LATCH_PIN -1
#define SNES_PAD_DATA_PIN -1
#endif



#include "CRC32.h"
#include "FlashPROM.h"

#include <cassert>
#include <cstring>
#include <memory>

#include "pico/platform.h"

// -----------------------------------------------------
// Default values
// -----------------------------------------------------

#define PREPROCESSOR_JOIN2(x, y) x ## y
#define PREPROCESSOR_JOIN(x, y) PREPROCESSOR_JOIN2(x, y)

#define INIT_UNSET_PROPERTY(parent, property, value) \
    if (!parent.PREPROCESSOR_JOIN(has_, property)) \
    { \
        parent.property = value; \
        parent.PREPROCESSOR_JOIN(has_, property) = true; \
    } \

#define INIT_UNSET_PROPERTY_STR(parent, property, str) \
    if (!parent.PREPROCESSOR_JOIN(has_, property)) \
    { \
        strncpy(parent.property, str, sizeof(parent.property)); \
        parent.property[sizeof(parent.property) - 1] = '\0'; \
        parent.PREPROCESSOR_JOIN(has_, property) = true; \
    } \

#define INIT_UNSET_PROPERTY_BYTES(parent, property, byteArray) \
    if (!parent.PREPROCESSOR_JOIN(has_, property)) \
    { \
        parent.property.size = sizeof(byteArray); \
        memcpy(parent.property.bytes, byteArray, std::min(sizeof(byteArray), sizeof(parent.property.bytes))); \
        parent.PREPROCESSOR_JOIN(has_, property) = true; \
    } \

#ifndef DEFAULT_INPUT_MODE
    #define DEFAULT_INPUT_MODE INPUT_MODE_XINPUT
#endif
#ifndef DEFAULT_INPUT_MODE_B1
    #define DEFAULT_INPUT_MODE_B1 INPUT_MODE_SWITCH
#endif
#ifndef DEFAULT_INPUT_MODE_B2
    #define DEFAULT_INPUT_MODE_B2 INPUT_MODE_XINPUT
#endif
#ifndef DEFAULT_INPUT_MODE_B3
    #define DEFAULT_INPUT_MODE_B3 INPUT_MODE_PS3
#endif
#ifndef DEFAULT_INPUT_MODE_B4
    #define DEFAULT_INPUT_MODE_B4 INPUT_MODE_PS4
#endif
#ifndef DEFAULT_INPUT_MODE_L1
    #define DEFAULT_INPUT_MODE_L1 -1
#endif
#ifndef DEFAULT_INPUT_MODE_L2
    #define DEFAULT_INPUT_MODE_L2 -1
#endif
#ifndef DEFAULT_INPUT_MODE_R1
    #define DEFAULT_INPUT_MODE_R1 -1
#endif
#ifndef DEFAULT_INPUT_MODE_R2
    #define DEFAULT_INPUT_MODE_R2 INPUT_MODE_KEYBOARD
#endif
#ifndef DEFAULT_DPAD_MODE
    #define DEFAULT_DPAD_MODE DPAD_MODE_DIGITAL
#endif
#ifndef DEFAULT_SOCD_MODE
    #define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
#endif
#ifndef DEFAULT_FORCED_SETUP_MODE
    #define DEFAULT_FORCED_SETUP_MODE FORCED_SETUP_MODE_OFF
#endif
#ifndef DEFAULT_LOCK_HOTKEYS
    #define DEFAULT_LOCK_HOTKEYS false
#endif
#ifndef DEFAULT_PS4CONTROLLER_TYPE
    #define DEFAULT_PS4CONTROLLER_TYPE PS4_CONTROLLER
#endif

#ifndef DEFAULT_DEBOUNCE_DELAY
    #define DEFAULT_DEBOUNCE_DELAY 5
#endif

#ifndef DEFAULT_PS4_REPORTHACK
    #define DEFAULT_PS4_REPORTHACK false
#endif

#ifndef DEFAULT_PS4AUTHENTICATION_TYPE
    #define DEFAULT_PS4AUTHENTICATION_TYPE INPUT_MODE_AUTH_TYPE_NONE
#endif

#ifndef DEFAULT_PS5AUTHENTICATION_TYPE
    #define DEFAULT_PS5AUTHENTICATION_TYPE INPUT_MODE_AUTH_TYPE_NONE
#endif

#ifndef DEFAULT_XINPUTAUTHENTICATION_TYPE
    #define DEFAULT_XINPUTAUTHENTICATION_TYPE INPUT_MODE_AUTH_TYPE_NONE
#endif

#ifndef DEFAULT_PS4_ID_MODE
    #define DEFAULT_PS4_ID_MODE PS4_ID_CONSOLE
#endif
#ifndef DEFAULT_GENERIC_DPAD_MODE
    #define DEFAULT_GENERIC_DPAD_MODE GENERIC_DPAD_HAT_AND_BUTTONS
#endif

#ifndef DEFAULT_SWITCH_PRO_CONTROLLER_TYPE
    #define DEFAULT_SWITCH_PRO_CONTROLLER_TYPE SWITCH_PRO_CONTROLLER_TYPE_PRO
#endif
#ifndef DEFAULT_SWITCH_PRO_COLOR_PRESET
    #define DEFAULT_SWITCH_PRO_COLOR_PRESET SWITCH_PRO_COLOR_STOCK
#endif
#ifndef DEFAULT_SWITCH_PRO_BODY_COLOR
    #define DEFAULT_SWITCH_PRO_BODY_COLOR 0x333333
#endif
#ifndef DEFAULT_SWITCH_PRO_BUTTONS_COLOR
    #define DEFAULT_SWITCH_PRO_BUTTONS_COLOR 0xFFFFFF
#endif
#ifndef DEFAULT_SWITCH_PRO_LEFT_GRIP_COLOR
    #define DEFAULT_SWITCH_PRO_LEFT_GRIP_COLOR 0xEC008C
#endif
#ifndef DEFAULT_SWITCH_PRO_RIGHT_GRIP_COLOR
    #define DEFAULT_SWITCH_PRO_RIGHT_GRIP_COLOR 0x00FF00
#endif
#ifndef DEFAULT_SWITCH_PRO_JOYCON_COLOR_PRESET
    #define DEFAULT_SWITCH_PRO_JOYCON_COLOR_PRESET SWITCH_PRO_JOYCON_COLOR_GREY
#endif
#ifndef DEFAULT_SWITCH_PRO_JOYCON_LEFT_BODY_COLOR
    #define DEFAULT_SWITCH_PRO_JOYCON_LEFT_BODY_COLOR 0x0AB9E6
#endif
#ifndef DEFAULT_SWITCH_PRO_JOYCON_LEFT_BUTTONS_COLOR
    #define DEFAULT_SWITCH_PRO_JOYCON_LEFT_BUTTONS_COLOR 0x0F0F0F
#endif
#ifndef DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BODY_COLOR
    #define DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BODY_COLOR 0xFF3C28
#endif
#ifndef DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BUTTONS_COLOR
    #define DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BUTTONS_COLOR 0x0F0F0F
#endif
#ifndef DEFAULT_SWITCH2_PRO_IDENTITY
    #define DEFAULT_SWITCH2_PRO_IDENTITY SWITCH2_PRO_IDENTITY_PRO
#endif
#ifndef DEFAULT_SWITCH2_PRO_COLOR_PRESET
    #define DEFAULT_SWITCH2_PRO_COLOR_PRESET SWITCH2_PRO_COLOR_STOCK
#endif
#ifndef DEFAULT_SWITCH2_PRO_BODY_COLOR
    #define DEFAULT_SWITCH2_PRO_BODY_COLOR 0x232323
#endif
#ifndef DEFAULT_SWITCH2_PRO_BUTTONS_COLOR
    #define DEFAULT_SWITCH2_PRO_BUTTONS_COLOR 0xA0A0A0
#endif
#ifndef DEFAULT_SWITCH2_PRO_HIGHLIGHT_COLOR
    #define DEFAULT_SWITCH2_PRO_HIGHLIGHT_COLOR 0xE6E6E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_GRIP_COLOR
    #define DEFAULT_SWITCH2_PRO_GRIP_COLOR 0x323232
#endif
#ifndef DEFAULT_SWITCH2_PRO_LEFT_SHOULDER_COLOR
    #define DEFAULT_SWITCH2_PRO_LEFT_SHOULDER_COLOR 0xE6E6E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_RIGHT_SHOULDER_COLOR
    #define DEFAULT_SWITCH2_PRO_RIGHT_SHOULDER_COLOR 0xE6E6E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_STICK_COLOR
    #define DEFAULT_SWITCH2_PRO_STICK_COLOR 0x232323
#endif
#ifndef DEFAULT_SWITCH2_PRO_LEFT_RING_COLOR
    #define DEFAULT_SWITCH2_PRO_LEFT_RING_COLOR 0xE6E6E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_RIGHT_RING_COLOR
    #define DEFAULT_SWITCH2_PRO_RIGHT_RING_COLOR 0xE6E6E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_COLOR_PRESET
    #define DEFAULT_SWITCH2_PRO_JOYCON2_COLOR_PRESET SWITCH2_PRO_JOYCON2_COLOR_STOCK
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BODY_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BODY_COLOR 0x323232
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BUTTONS_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BUTTONS_COLOR 0xAAAAAA
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_ACCENT_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_ACCENT_COLOR 0x9BE1E6
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_STICK_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_STICK_COLOR 0x323232
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BODY_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BODY_COLOR 0x323232
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BUTTONS_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BUTTONS_COLOR 0xAAAAAA
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_ACCENT_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_ACCENT_COLOR 0xFF8C5F
#endif
#ifndef DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_STICK_COLOR
    #define DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_STICK_COLOR 0x323232
#endif

#ifndef DEFAULT_ERM_RUMBLE_MODE
    #ifdef ERM_RUMBLE_MODE
        #define DEFAULT_ERM_RUMBLE_MODE ERM_RUMBLE_MODE
    #else
        #define DEFAULT_ERM_RUMBLE_MODE 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_MOTOR_FLOOR
    #ifdef ERM_RUMBLE_MOTOR_FLOOR
        #define DEFAULT_ERM_RUMBLE_MOTOR_FLOOR ERM_RUMBLE_MOTOR_FLOOR
    #elif defined(ERM_RUMBLE_LEFT_MOTOR_FLOOR)
        #define DEFAULT_ERM_RUMBLE_MOTOR_FLOOR ERM_RUMBLE_LEFT_MOTOR_FLOOR
    #else
        #define DEFAULT_ERM_RUMBLE_MOTOR_FLOOR 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_KICKSTART_MS
    #ifdef ERM_RUMBLE_KICKSTART_MS
        #define DEFAULT_ERM_RUMBLE_KICKSTART_MS ERM_RUMBLE_KICKSTART_MS
    #elif defined(ERM_RUMBLE_LEFT_KICKSTART_MS)
        #define DEFAULT_ERM_RUMBLE_KICKSTART_MS ERM_RUMBLE_LEFT_KICKSTART_MS
    #else
        #define DEFAULT_ERM_RUMBLE_KICKSTART_MS 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_SUSTAIN_MS
    #ifdef ERM_RUMBLE_SUSTAIN_MS
        #define DEFAULT_ERM_RUMBLE_SUSTAIN_MS ERM_RUMBLE_SUSTAIN_MS
    #elif defined(ERM_RUMBLE_LEFT_SUSTAIN_MS)
        #define DEFAULT_ERM_RUMBLE_SUSTAIN_MS ERM_RUMBLE_LEFT_SUSTAIN_MS
    #else
        #define DEFAULT_ERM_RUMBLE_SUSTAIN_MS 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_RIGHT_MOTOR_FLOOR
    #ifdef ERM_RUMBLE_RIGHT_MOTOR_FLOOR
        #define DEFAULT_ERM_RUMBLE_RIGHT_MOTOR_FLOOR ERM_RUMBLE_RIGHT_MOTOR_FLOOR
    #else
        #define DEFAULT_ERM_RUMBLE_RIGHT_MOTOR_FLOOR 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_RIGHT_KICKSTART_MS
    #ifdef ERM_RUMBLE_RIGHT_KICKSTART_MS
        #define DEFAULT_ERM_RUMBLE_RIGHT_KICKSTART_MS ERM_RUMBLE_RIGHT_KICKSTART_MS
    #else
        #define DEFAULT_ERM_RUMBLE_RIGHT_KICKSTART_MS 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_RIGHT_SUSTAIN_MS
    #ifdef ERM_RUMBLE_RIGHT_SUSTAIN_MS
        #define DEFAULT_ERM_RUMBLE_RIGHT_SUSTAIN_MS ERM_RUMBLE_RIGHT_SUSTAIN_MS
    #else
        #define DEFAULT_ERM_RUMBLE_RIGHT_SUSTAIN_MS 0
    #endif
#endif
#ifndef DEFAULT_ERM_RUMBLE_BOOST_PERCENT
    #ifdef ERM_RUMBLE_BOOST_PERCENT
        #define DEFAULT_ERM_RUMBLE_BOOST_PERCENT ERM_RUMBLE_BOOST_PERCENT
    #else
        #define DEFAULT_ERM_RUMBLE_BOOST_PERCENT 0
    #endif
#endif

#ifndef DEFAULT_USB_DESC_OVERRIDE
   #define DEFAULT_USB_DESC_OVERRIDE false
#endif

#ifndef DEFAULT_USB_DESC_PRODUCT
   #define DEFAULT_USB_DESC_PRODUCT "GP2040-CE (Custom)"
#endif

#ifndef DEFAULT_USB_DESC_MANUFACTURER
   #define DEFAULT_USB_DESC_MANUFACTURER "Open Stick Community"
#endif

#ifndef DEFAULT_USB_DESC_VERSION
   #define DEFAULT_USB_DESC_VERSION "1.0"
#endif

#ifndef DEFAULT_USB_ID_OVERRIDE
   #define DEFAULT_USB_ID_OVERRIDE false
#endif

#ifndef DEFAULT_USB_VENDOR_ID
   #define DEFAULT_USB_VENDOR_ID 0x10C4
#endif

#ifndef DEFAULT_USB_PRODUCT_ID
   #define DEFAULT_USB_PRODUCT_ID 0x82C0
#endif

#ifndef MINI_MENU_GAMEPAD_INPUT
   #define MINI_MENU_GAMEPAD_INPUT 0
#endif

#ifndef GPIO_INVERTED_PIN_MASK
    #define GPIO_INVERTED_PIN_MASK 0
#endif

#ifndef GPIO_PIN_00
    #define GPIO_PIN_00 GpioAction::NONE
#endif
#ifndef GPIO_PIN_01
    #define GPIO_PIN_01 GpioAction::NONE
#endif
#ifndef GPIO_PIN_02
    #define GPIO_PIN_02 GpioAction::NONE
#endif
#ifndef GPIO_PIN_03
    #define GPIO_PIN_03 GpioAction::NONE
#endif
#ifndef GPIO_PIN_04
    #define GPIO_PIN_04 GpioAction::NONE
#endif
#ifndef GPIO_PIN_05
    #define GPIO_PIN_05 GpioAction::NONE
#endif
#ifndef GPIO_PIN_06
    #define GPIO_PIN_06 GpioAction::NONE
#endif
#ifndef GPIO_PIN_07
    #define GPIO_PIN_07 GpioAction::NONE
#endif
#ifndef GPIO_PIN_08
    #define GPIO_PIN_08 GpioAction::NONE
#endif
#ifndef GPIO_PIN_09
    #define GPIO_PIN_09 GpioAction::NONE
#endif
#ifndef GPIO_PIN_10
    #define GPIO_PIN_10 GpioAction::NONE
#endif
#ifndef GPIO_PIN_11
    #define GPIO_PIN_11 GpioAction::NONE
#endif
#ifndef GPIO_PIN_12
    #define GPIO_PIN_12 GpioAction::NONE
#endif
#ifndef GPIO_PIN_13
    #define GPIO_PIN_13 GpioAction::NONE
#endif
#ifndef GPIO_PIN_14
    #define GPIO_PIN_14 GpioAction::NONE
#endif
#ifndef GPIO_PIN_15
    #define GPIO_PIN_15 GpioAction::NONE
#endif
#ifndef GPIO_PIN_16
    #define GPIO_PIN_16 GpioAction::NONE
#endif
#ifndef GPIO_PIN_17
    #define GPIO_PIN_17 GpioAction::NONE
#endif
#ifndef GPIO_PIN_18
    #define GPIO_PIN_18 GpioAction::NONE
#endif
#ifndef GPIO_PIN_19
    #define GPIO_PIN_19 GpioAction::NONE
#endif
#ifndef GPIO_PIN_20
    #define GPIO_PIN_20 GpioAction::NONE
#endif
#ifndef GPIO_PIN_21
    #define GPIO_PIN_21 GpioAction::NONE
#endif
#ifndef GPIO_PIN_22
    #define GPIO_PIN_22 GpioAction::NONE
#endif
#ifndef GPIO_PIN_23
    #define GPIO_PIN_23 GpioAction::NONE
#endif
#ifndef GPIO_PIN_24
    #define GPIO_PIN_24 GpioAction::NONE
#endif
#ifndef GPIO_PIN_25
    #define GPIO_PIN_25 GpioAction::NONE
#endif
#ifndef GPIO_PIN_26
    #define GPIO_PIN_26 GpioAction::NONE
#endif
#ifndef GPIO_PIN_27
    #define GPIO_PIN_27 GpioAction::NONE
#endif
#ifndef GPIO_PIN_28
    #define GPIO_PIN_28 GpioAction::NONE
#endif
#ifndef GPIO_PIN_29
    #define GPIO_PIN_29 GpioAction::NONE
#endif
#ifndef GPIO_PIN_30
    #define GPIO_PIN_30 GpioAction::NONE
#endif
#ifndef GPIO_PIN_31
    #define GPIO_PIN_31 GpioAction::NONE
#endif
#ifndef GPIO_PIN_32
    #define GPIO_PIN_32 GpioAction::NONE
#endif
#ifndef GPIO_PIN_33
    #define GPIO_PIN_33 GpioAction::NONE
#endif
#ifndef GPIO_PIN_34
    #define GPIO_PIN_34 GpioAction::NONE
#endif
#ifndef GPIO_PIN_35
    #define GPIO_PIN_35 GpioAction::NONE
#endif
#ifndef GPIO_PIN_36
    #define GPIO_PIN_36 GpioAction::NONE
#endif
#ifndef GPIO_PIN_37
    #define GPIO_PIN_37 GpioAction::NONE
#endif
#ifndef GPIO_PIN_38
    #define GPIO_PIN_38 GpioAction::NONE
#endif
#ifndef GPIO_PIN_39
    #define GPIO_PIN_39 GpioAction::NONE
#endif
#ifndef GPIO_PIN_40
    #define GPIO_PIN_40 GpioAction::NONE
#endif
#ifndef GPIO_PIN_41
    #define GPIO_PIN_41 GpioAction::NONE
#endif
#ifndef GPIO_PIN_42
    #define GPIO_PIN_42 GpioAction::NONE
#endif
#ifndef GPIO_PIN_43
    #define GPIO_PIN_43 GpioAction::NONE
#endif
#ifndef GPIO_PIN_44
    #define GPIO_PIN_44 GpioAction::NONE
#endif
#ifndef GPIO_PIN_45
    #define GPIO_PIN_45 GpioAction::NONE
#endif
#ifndef GPIO_PIN_46
    #define GPIO_PIN_46 GpioAction::NONE
#endif
#ifndef GPIO_PIN_47
    #define GPIO_PIN_47 GpioAction::NONE
#endif

#define MAX_PROFILES (uint8_t)6

// -----------------------------------------------------
// Migration leftovers
// -----------------------------------------------------

#ifndef EXTRA_BUTTON_PIN
    #define EXTRA_BUTTON_PIN -1
#endif
#ifndef EXTRA_BUTTON_MASK
    #define EXTRA_BUTTON_MASK 0
#endif

void ConfigUtils::initUnsetPropertiesWithDefaults(Config& config)
{
    INIT_UNSET_PROPERTY_STR(config, boardVersion, GP2040VERSION);
    INIT_UNSET_PROPERTY_STR(config, boardConfig, GP2040_BOARDCONFIG);

    // gamepadOptions
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputMode, DEFAULT_INPUT_MODE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, dpadMode, DEFAULT_DPAD_MODE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, socdMode, DEFAULT_SOCD_MODE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, invertXAxis, false);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchTpShareForDs4, false);
    INIT_UNSET_PROPERTY(config.gamepadOptions, lockHotkeys, DEFAULT_LOCK_HOTKEYS);
    INIT_UNSET_PROPERTY(config.gamepadOptions, fourWayMode, false);
    INIT_UNSET_PROPERTY(config.gamepadOptions, profileNumber, 1);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ps4ControllerType, DEFAULT_PS4CONTROLLER_TYPE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, debounceDelay, DEFAULT_DEBOUNCE_DELAY);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeB1, DEFAULT_INPUT_MODE_B1);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeB2, DEFAULT_INPUT_MODE_B2);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeB3, DEFAULT_INPUT_MODE_B3);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeB4, DEFAULT_INPUT_MODE_B4);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeL1, DEFAULT_INPUT_MODE_L1);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeL2, DEFAULT_INPUT_MODE_L2);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeR1, DEFAULT_INPUT_MODE_R1);
    INIT_UNSET_PROPERTY(config.gamepadOptions, inputModeR2, DEFAULT_INPUT_MODE_R2);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ps4AuthType, DEFAULT_PS4AUTHENTICATION_TYPE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ps5AuthType, DEFAULT_PS5AUTHENTICATION_TYPE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, xinputAuthType, DEFAULT_XINPUTAUTHENTICATION_TYPE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ps4ControllerIDMode, DEFAULT_PS4_ID_MODE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProControllerType, DEFAULT_SWITCH_PRO_CONTROLLER_TYPE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProColorPreset, DEFAULT_SWITCH_PRO_COLOR_PRESET);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProBodyColor, DEFAULT_SWITCH_PRO_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProButtonsColor, DEFAULT_SWITCH_PRO_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProLeftGripColor, DEFAULT_SWITCH_PRO_LEFT_GRIP_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProRightGripColor, DEFAULT_SWITCH_PRO_RIGHT_GRIP_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProJoyConColorPreset, DEFAULT_SWITCH_PRO_JOYCON_COLOR_PRESET);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProJoyConLeftBodyColor, DEFAULT_SWITCH_PRO_JOYCON_LEFT_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProJoyConLeftButtonsColor, DEFAULT_SWITCH_PRO_JOYCON_LEFT_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProJoyConRightBodyColor, DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switchProJoyConRightButtonsColor, DEFAULT_SWITCH_PRO_JOYCON_RIGHT_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProIdentity, DEFAULT_SWITCH2_PRO_IDENTITY);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProColorPreset, DEFAULT_SWITCH2_PRO_COLOR_PRESET);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProBodyColor, DEFAULT_SWITCH2_PRO_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProButtonsColor, DEFAULT_SWITCH2_PRO_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProHighlightColor, DEFAULT_SWITCH2_PRO_HIGHLIGHT_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProGripColor, DEFAULT_SWITCH2_PRO_GRIP_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProLeftShoulderColor, DEFAULT_SWITCH2_PRO_LEFT_SHOULDER_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProRightShoulderColor, DEFAULT_SWITCH2_PRO_RIGHT_SHOULDER_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProStickColor, DEFAULT_SWITCH2_PRO_STICK_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProLeftRingColor, DEFAULT_SWITCH2_PRO_LEFT_RING_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProRightRingColor, DEFAULT_SWITCH2_PRO_RIGHT_RING_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2ColorPreset, DEFAULT_SWITCH2_PRO_JOYCON2_COLOR_PRESET);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2LeftBodyColor, DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2LeftButtonsColor, DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2LeftAccentColor, DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_ACCENT_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2LeftStickColor, DEFAULT_SWITCH2_PRO_JOYCON2_LEFT_STICK_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2RightBodyColor, DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BODY_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2RightButtonsColor, DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_BUTTONS_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2RightAccentColor, DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_ACCENT_COLOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, switch2ProJoyCon2RightStickColor, DEFAULT_SWITCH2_PRO_JOYCON2_RIGHT_STICK_COLOR);
    if (!config.gamepadOptions.has_switch2ProUserCalib) {
        memset(config.gamepadOptions.switch2ProUserCalib.bytes, 0xFF, sizeof(config.gamepadOptions.switch2ProUserCalib.bytes));
        config.gamepadOptions.switch2ProUserCalib.size = sizeof(config.gamepadOptions.switch2ProUserCalib.bytes);
        config.gamepadOptions.has_switch2ProUserCalib = true;
    }
    INIT_UNSET_PROPERTY(config.gamepadOptions, genericDpadMode, DEFAULT_GENERIC_DPAD_MODE);
    if (!config.gamepadOptions.has_ermRumbleMode) {
        config.gamepadOptions.has_ermRumbleMotorFloor = false;
        config.gamepadOptions.has_ermRumbleKickstartMs = false;
        config.gamepadOptions.has_ermRumbleSustainMs = false;
        config.gamepadOptions.has_ermRumbleBoostPercent = false;
    }
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleMode, DEFAULT_ERM_RUMBLE_MODE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleMotorFloor, DEFAULT_ERM_RUMBLE_MOTOR_FLOOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleKickstartMs, DEFAULT_ERM_RUMBLE_KICKSTART_MS);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleSustainMs, DEFAULT_ERM_RUMBLE_SUSTAIN_MS);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleRightMotorFloor, DEFAULT_ERM_RUMBLE_RIGHT_MOTOR_FLOOR);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleRightKickstartMs, DEFAULT_ERM_RUMBLE_RIGHT_KICKSTART_MS);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleRightSustainMs, DEFAULT_ERM_RUMBLE_RIGHT_SUSTAIN_MS);
    INIT_UNSET_PROPERTY(config.gamepadOptions, ermRumbleBoostPercent, DEFAULT_ERM_RUMBLE_BOOST_PERCENT);
    INIT_UNSET_PROPERTY(config.gamepadOptions, usbDescOverride, DEFAULT_USB_DESC_OVERRIDE);
    INIT_UNSET_PROPERTY_STR(config.gamepadOptions, usbDescProduct, DEFAULT_USB_DESC_PRODUCT);
    INIT_UNSET_PROPERTY_STR(config.gamepadOptions, usbDescManufacturer, DEFAULT_USB_DESC_MANUFACTURER);
    INIT_UNSET_PROPERTY_STR(config.gamepadOptions, usbDescVersion, DEFAULT_USB_DESC_VERSION);
    INIT_UNSET_PROPERTY(config.gamepadOptions, usbOverrideID, DEFAULT_USB_ID_OVERRIDE);
    INIT_UNSET_PROPERTY(config.gamepadOptions, usbVendorID, DEFAULT_USB_VENDOR_ID);
    INIT_UNSET_PROPERTY(config.gamepadOptions, usbProductID, DEFAULT_USB_PRODUCT_ID);
    INIT_UNSET_PROPERTY(config.gamepadOptions, miniMenuGamepadInput, MINI_MENU_GAMEPAD_INPUT);

    // hotkeyOptions
    HotkeyOptions& hotkeyOptions = config.hotkeyOptions;
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey01, auxMask, HOTKEY_01_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey01, buttonsMask, HOTKEY_01_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey01, dpadMask, HOTKEY_01_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey01, action, GamepadHotkey(HOTKEY_01_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey02, auxMask, HOTKEY_02_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey02, buttonsMask, HOTKEY_02_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey02, dpadMask, HOTKEY_02_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey02, action, GamepadHotkey(HOTKEY_02_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey03, auxMask, HOTKEY_03_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey03, buttonsMask, HOTKEY_03_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey03, dpadMask, HOTKEY_03_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey03, action, GamepadHotkey(HOTKEY_03_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey04, auxMask, HOTKEY_04_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey04, buttonsMask, HOTKEY_04_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey04, dpadMask, HOTKEY_04_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey04, action, GamepadHotkey(HOTKEY_04_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey05, auxMask, HOTKEY_05_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey05, buttonsMask, HOTKEY_05_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey05, dpadMask, HOTKEY_05_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey05, action, GamepadHotkey(HOTKEY_05_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey06, auxMask, HOTKEY_06_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey06, buttonsMask, HOTKEY_06_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey06, dpadMask, HOTKEY_06_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey06, action, GamepadHotkey(HOTKEY_06_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey07, auxMask, HOTKEY_07_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey07, buttonsMask, HOTKEY_07_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey07, dpadMask, HOTKEY_07_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey07, action, GamepadHotkey(HOTKEY_07_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey08, auxMask, HOTKEY_08_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey08, buttonsMask, HOTKEY_08_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey08, dpadMask, HOTKEY_08_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey08, action, GamepadHotkey(HOTKEY_08_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey09, auxMask, HOTKEY_09_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey09, buttonsMask, HOTKEY_09_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey09, dpadMask, HOTKEY_09_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey09, action, GamepadHotkey(HOTKEY_09_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey10, auxMask, HOTKEY_10_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey10, buttonsMask, HOTKEY_10_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey10, dpadMask, HOTKEY_10_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey10, action, GamepadHotkey(HOTKEY_10_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey11, auxMask, HOTKEY_11_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey11, buttonsMask, HOTKEY_11_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey11, dpadMask, HOTKEY_11_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey11, action, GamepadHotkey(HOTKEY_11_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey12, auxMask, HOTKEY_12_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey12, buttonsMask, HOTKEY_12_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey12, dpadMask, HOTKEY_12_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey12, action, GamepadHotkey(HOTKEY_12_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey13, auxMask, HOTKEY_13_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey13, buttonsMask, HOTKEY_13_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey13, dpadMask, HOTKEY_13_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey13, action, GamepadHotkey(HOTKEY_13_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey14, auxMask, HOTKEY_14_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey14, buttonsMask, HOTKEY_14_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey14, dpadMask, HOTKEY_14_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey14, action, GamepadHotkey(HOTKEY_14_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey15, auxMask, HOTKEY_15_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey15, buttonsMask, HOTKEY_15_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey15, dpadMask, HOTKEY_15_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey15, action, GamepadHotkey(HOTKEY_15_ACTION));
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey16, auxMask, HOTKEY_16_AUX_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey16, buttonsMask, HOTKEY_16_BUTTONS_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey16, dpadMask, HOTKEY_16_DPAD_MASK);
    INIT_UNSET_PROPERTY(hotkeyOptions.hotkey16, action, GamepadHotkey(HOTKEY_16_ACTION));

    // forcedSetupMode
    INIT_UNSET_PROPERTY(config.forcedSetupOptions, mode, DEFAULT_FORCED_SETUP_MODE);

    // keyboardMapping
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyDpadUp, KEY_DPAD_UP);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyDpadDown, KEY_DPAD_DOWN);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyDpadRight, KEY_DPAD_RIGHT);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyDpadLeft, KEY_DPAD_LEFT);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonB1, KEY_BUTTON_B1);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonB2, KEY_BUTTON_B2);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonR2, KEY_BUTTON_R2);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonL2, KEY_BUTTON_L2);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonB3, KEY_BUTTON_B3);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonB4, KEY_BUTTON_B4);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonR1, KEY_BUTTON_R1);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonL1, KEY_BUTTON_L1);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonS1, KEY_BUTTON_S1);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonS2, KEY_BUTTON_S2);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonL3, KEY_BUTTON_L3);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonR3, KEY_BUTTON_R3);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonA1, KEY_BUTTON_A1);
    INIT_UNSET_PROPERTY(config.keyboardMapping, keyButtonA2, KEY_BUTTON_A2);

    const auto initPinInversion = [](GpioMappings& mappings) {
        if (mappings.has_pinInversionEnabled)
            return;
        const uint64_t invertedPinMask = GPIO_INVERTED_PIN_MASK;
        mappings.pinInversionEnabled = invertedPinMask != 0;
        mappings.has_pinInversionEnabled = true;
        mappings.invertedPins_count = 0;
        for (uint32_t pin = 0; pin < NUM_BANK0_GPIOS; pin++) {
            if (invertedPinMask & (uint64_t{1} << pin))
                mappings.invertedPins[mappings.invertedPins_count++] = pin;
        }
    };
    initPinInversion(config.gpioMappings);

    for (GpioMappings& profile : config.profileOptions.gpioMappingsSets) {
        if (!profile.has_keyboardMapping) {
            profile.keyboardMapping = config.keyboardMapping;
            profile.has_keyboardMapping = true;
        }
        initPinInversion(profile);
    }

    // displayOptions
    INIT_UNSET_PROPERTY(config.displayOptions, enabled, !!HAS_I2C_DISPLAY);
    INIT_UNSET_PROPERTY(config.displayOptions, deprecatedI2cBlock, 0);
    INIT_UNSET_PROPERTY(config.displayOptions, deprecatedI2cSDAPin, -1);
    INIT_UNSET_PROPERTY(config.displayOptions, deprecatedI2cSCLPin, -1);
    INIT_UNSET_PROPERTY(config.displayOptions, deprecatedI2cAddress, DISPLAY_I2C_ADDR);
    INIT_UNSET_PROPERTY(config.displayOptions, deprecatedI2cSpeed, I2C_SPEED);
    INIT_UNSET_PROPERTY(config.displayOptions, buttonLayout, BUTTON_LAYOUT);
    INIT_UNSET_PROPERTY(config.displayOptions, buttonLayoutRight, BUTTON_LAYOUT_RIGHT);

    INIT_UNSET_PROPERTY(config.displayOptions, turnOffWhenSuspended, DISPLAY_TURN_OFF_WHEN_SUSPENDED);
    INIT_UNSET_PROPERTY(config.displayOptions, inputMode, DISPLAY_INPUT_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, turboMode, DISPLAY_TURBO_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, dpadMode, DISPLAY_DPAD_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, socdMode, DISPLAY_SOCD_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, macroMode, DISPLAY_MACRO_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, profileMode, DISPLAY_PROFILE_MODE);
    INIT_UNSET_PROPERTY(config.displayOptions, inputHistoryEnabled, !!INPUT_HISTORY_ENABLED);
    INIT_UNSET_PROPERTY(config.displayOptions, inputHistoryLength, INPUT_HISTORY_LENGTH);
    INIT_UNSET_PROPERTY(config.displayOptions, inputHistoryCol, INPUT_HISTORY_COL);
    INIT_UNSET_PROPERTY(config.displayOptions, inputHistoryRow, INPUT_HISTORY_ROW);

    INIT_UNSET_PROPERTY(config.displayOptions, splashMode, static_cast<SplashMode>(SPLASH_MODE));
    INIT_UNSET_PROPERTY(config.displayOptions, splashChoice, static_cast<SplashChoice>(SPLASH_CHOICE));
    INIT_UNSET_PROPERTY(config.displayOptions, splashDuration, SPLASH_DURATION);
	const unsigned char defaultSplash[] = { DEFAULT_SPLASH };
    INIT_UNSET_PROPERTY_BYTES(config.displayOptions, splashImage, defaultSplash);

    INIT_UNSET_PROPERTY(config.displayOptions, size, DISPLAY_SIZE);
    INIT_UNSET_PROPERTY(config.displayOptions, flip, DISPLAY_FLIP);
    INIT_UNSET_PROPERTY(config.displayOptions, invert, !!DISPLAY_INVERT);
    INIT_UNSET_PROPERTY(config.displayOptions, displaySaverTimeout, DISPLAY_SAVER_TIMEOUT);
    INIT_UNSET_PROPERTY(config.displayOptions, displaySaverMode, static_cast<DisplaySaverMode>(DISPLAY_SAVER_MODE));
    INIT_UNSET_PROPERTY(config.displayOptions, buttonLayoutOrientation, static_cast<ButtonLayoutOrientation>(DISPLAY_LAYOUT_ORIENTATION));
    INIT_UNSET_PROPERTY(config.displayOptions, contrast, DISPLAY_CONTRAST);

    // peripheralOptions
    PeripheralOptions& peripheralOptions = config.peripheralOptions;
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C0, enabled, (!!I2C0_ENABLED));
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C0, sda, (!!I2C0_ENABLED) ? I2C0_PIN_SDA : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C0, scl, (!!I2C0_ENABLED) ? I2C0_PIN_SCL : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C0, speed, I2C0_SPEED);
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C1, enabled, (!!I2C1_ENABLED));
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C1, sda, (!!I2C1_ENABLED) ? I2C1_PIN_SDA : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C1, scl, (!!I2C1_ENABLED) ? I2C1_PIN_SCL : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockI2C1, speed, I2C1_SPEED);

    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI0, enabled, (!!SPI0_ENABLED));
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI0, rx, (!!SPI0_ENABLED) ?  SPI0_PIN_RX : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI0, cs, (!!SPI0_ENABLED) ?  SPI0_PIN_CS : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI0, sck, (!!SPI0_ENABLED) ? SPI0_PIN_SCK : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI0, tx, (!!SPI0_ENABLED) ?  SPI0_PIN_TX : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI1, enabled, (!!SPI1_ENABLED));
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI1, rx, (!!SPI1_ENABLED) ?  SPI1_PIN_RX : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI1, cs, (!!SPI1_ENABLED) ?  SPI1_PIN_CS : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI1, sck, (!!SPI1_ENABLED) ? SPI1_PIN_SCK : -1);
    INIT_UNSET_PROPERTY(peripheralOptions.blockSPI1, tx, (!!SPI1_ENABLED) ?  SPI1_PIN_TX : -1);

    INIT_UNSET_PROPERTY(peripheralOptions.blockUSB0, enabled, USB_PERIPHERAL_ENABLED);
    INIT_UNSET_PROPERTY(peripheralOptions.blockUSB0, dp, USB_PERIPHERAL_PIN_DPLUS);
    INIT_UNSET_PROPERTY(peripheralOptions.blockUSB0, order, USB_PERIPHERAL_PIN_ORDER);
    INIT_UNSET_PROPERTY(peripheralOptions.blockUSB0, enable5v, USB_PERIPHERAL_PIN_5V);

    // ledOptions
    INIT_UNSET_PROPERTY(config.ledOptions, dataPin, BOARD_LEDS_PIN);
    INIT_UNSET_PROPERTY(config.ledOptions, brightnessMaximum, LED_BRIGHTNESS_MAXIMUM);

    // animationOptions
    INIT_UNSET_PROPERTY(config.animationOptions, brightness, LEDS_BRIGHTNESS);
    config.animationOptions.profiles_count = 0;

    // addonOptions.bootselButtonOptions
    INIT_UNSET_PROPERTY(config.addonOptions.bootselButtonOptions, enabled, !!BOOTSEL_BUTTON_ENABLED);
    INIT_UNSET_PROPERTY(config.addonOptions.bootselButtonOptions, buttonMap, BOOTSEL_BUTTON_MASK);

    // addonOptions.onBoardLedOptions
    INIT_UNSET_PROPERTY(config.addonOptions.onBoardLedOptions, enabled, !!BOARD_LED_ENABLED);
    INIT_UNSET_PROPERTY(config.addonOptions.onBoardLedOptions, mode, BOARD_LED_TYPE);

    // addonOptions.gamepadUSBHostOptions
    INIT_UNSET_PROPERTY(config.addonOptions.gamepadUSBHostOptions, enabled, GAMEPAD_USB_HOST_ENABLED);
}


// -----------------------------------------------------
// migrations
// used for when we might need to populate configs with
// something *other than* the board defaults
// -----------------------------------------------------

// convert core and addon pin mappings to GPIO mapping config
// NOTE: this also handles initializations for a blank config! if/when the deprecated
// pin mappings go away, the remainder of this code should go in there (there was no point
// in duplicating it right now)
void gpioMappingsMigrationCore(Config& config)
{
    PinMappings& deprecatedPinMappings = config.deprecatedPinMappings;
    ExtraButtonOptions& extraButtonOptions = config.addonOptions.deprecatedExtraButtonOptions;
    DualDirectionalOptions& ddiOptions = config.addonOptions.dualDirectionalOptions;
    SliderOptions& jsSliderOptions = config.addonOptions.deprecatedSliderOptions;
    SOCDSliderOptions& socdSliderOptions = config.addonOptions.socdSliderOptions;
    PeripheralOptions& peripheralOptions = config.peripheralOptions;
    KeyboardHostOptions& keyboardHostOptions = config.addonOptions.keyboardHostOptions;
    PSPassthroughOptions& psPassthroughOptions = config.addonOptions.psPassthroughOptions;
    FocusModeOptions& focusModeOptions = config.addonOptions.focusModeOptions;
    ReverseOptions& reverseOptions = config.addonOptions.reverseOptions;

    const auto gamepadMaskToGpioAction = [&](Mask_t gpMask) -> GpioAction
    {
        switch (gpMask)
        {
            case GAMEPAD_MASK_DU:
                return GpioAction::BUTTON_PRESS_UP;
            case GAMEPAD_MASK_DD:
                return GpioAction::BUTTON_PRESS_DOWN;
            case GAMEPAD_MASK_DL:
                return GpioAction::BUTTON_PRESS_LEFT;
            case GAMEPAD_MASK_DR:
                return GpioAction::BUTTON_PRESS_RIGHT;
            case GAMEPAD_MASK_B1:
                return GpioAction::BUTTON_PRESS_B1;
            case GAMEPAD_MASK_B2:
                return GpioAction::BUTTON_PRESS_B2;
            case GAMEPAD_MASK_B3:
                return GpioAction::BUTTON_PRESS_B3;
            case GAMEPAD_MASK_B4:
                return GpioAction::BUTTON_PRESS_B4;
            case GAMEPAD_MASK_L1:
                return GpioAction::BUTTON_PRESS_L1;
            case GAMEPAD_MASK_R1:
                return GpioAction::BUTTON_PRESS_R1;
            case GAMEPAD_MASK_L2:
                return GpioAction::BUTTON_PRESS_L2;
            case GAMEPAD_MASK_R2:
                return GpioAction::BUTTON_PRESS_R2;
            case GAMEPAD_MASK_S1:
                return GpioAction::BUTTON_PRESS_S1;
            case GAMEPAD_MASK_S2:
                return GpioAction::BUTTON_PRESS_S2;
            case GAMEPAD_MASK_L3:
                return GpioAction::BUTTON_PRESS_L3;
            case GAMEPAD_MASK_R3:
                return GpioAction::BUTTON_PRESS_R3;
            case GAMEPAD_MASK_A1:
                return GpioAction::BUTTON_PRESS_A1;
            case GAMEPAD_MASK_A2:
                return GpioAction::BUTTON_PRESS_A2;
            case AUX_MASK_FUNCTION:
                return GpioAction::BUTTON_PRESS_FN;
            default:
                return GpioAction::NONE;
        }
    };

    // create an array of the old
    GpioAction actions[NUM_BANK0_GPIOS] = {GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
#if NUM_BANK0_GPIOS > 32
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
                                           GpioAction::NONE, GpioAction::NONE, GpioAction::NONE,
#endif
                                           };

    // flag additional pins as being used by an addon not managed here
    const auto markAddonPinIfUsed = [&](Pin_t gpPin) -> void {
        if (isValidPin(gpPin))
            actions[gpPin] = GpioAction::ASSIGNED_TO_ADDON;
    };

    // From Protobuf or Board Config
    const auto fromProtoBuf = [&](bool isInProtobuf, Pin_t *protobufEntry, GpioAction action) -> void {
        // get the core config value for a pin either from protobuf or, failing that, BoardConfig.h
        if (isInProtobuf) {
            if (*protobufEntry >= 0 && *protobufEntry < 30) {
                actions[*protobufEntry] = action;
                *protobufEntry = -1;
            }
        }
    };

    const auto fromBoardConfig = [&](Pin_t pinAssign, GpioAction action) -> void {
        if (actions[pinAssign] == GpioAction::NONE && isValidPin(pinAssign)) {
            actions[pinAssign] = action;
        }
    };

    fromProtoBuf(deprecatedPinMappings.has_pinDpadUp,    &deprecatedPinMappings.pinDpadUp,    GpioAction::BUTTON_PRESS_UP);
    fromProtoBuf(deprecatedPinMappings.has_pinDpadDown,  &deprecatedPinMappings.pinDpadDown,  GpioAction::BUTTON_PRESS_DOWN);
    fromProtoBuf(deprecatedPinMappings.has_pinDpadLeft,  &deprecatedPinMappings.pinDpadLeft,  GpioAction::BUTTON_PRESS_LEFT);
    fromProtoBuf(deprecatedPinMappings.has_pinDpadRight, &deprecatedPinMappings.pinDpadRight, GpioAction::BUTTON_PRESS_RIGHT);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonB1,  &deprecatedPinMappings.pinButtonB1,  GpioAction::BUTTON_PRESS_B1);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonB2,  &deprecatedPinMappings.pinButtonB2,  GpioAction::BUTTON_PRESS_B2);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonB3,  &deprecatedPinMappings.pinButtonB3,  GpioAction::BUTTON_PRESS_B3);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonB4,  &deprecatedPinMappings.pinButtonB4,  GpioAction::BUTTON_PRESS_B4);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonL1,  &deprecatedPinMappings.pinButtonL1,  GpioAction::BUTTON_PRESS_L1);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonR1,  &deprecatedPinMappings.pinButtonR1,  GpioAction::BUTTON_PRESS_R1);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonL2,  &deprecatedPinMappings.pinButtonL2,  GpioAction::BUTTON_PRESS_L2);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonR2,  &deprecatedPinMappings.pinButtonR2,  GpioAction::BUTTON_PRESS_R2);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonS1,  &deprecatedPinMappings.pinButtonS1,  GpioAction::BUTTON_PRESS_S1);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonS2,  &deprecatedPinMappings.pinButtonS2,  GpioAction::BUTTON_PRESS_S2);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonL3,  &deprecatedPinMappings.pinButtonL3,  GpioAction::BUTTON_PRESS_L3);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonR3,  &deprecatedPinMappings.pinButtonR3,  GpioAction::BUTTON_PRESS_R3);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonA1,  &deprecatedPinMappings.pinButtonA1,  GpioAction::BUTTON_PRESS_A1);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonA2,  &deprecatedPinMappings.pinButtonA2,  GpioAction::BUTTON_PRESS_A2);
    fromProtoBuf(deprecatedPinMappings.has_pinButtonFn,  &deprecatedPinMappings.pinButtonFn,  GpioAction::BUTTON_PRESS_FN);

    // convert extra pin mapping to GPIO mapping config
    if (extraButtonOptions.enabled && isValidPin(extraButtonOptions.pin)) {
        // previous config had a value we haven't migrated yet, it can/should apply in the new config
        actions[extraButtonOptions.pin] = gamepadMaskToGpioAction(extraButtonOptions.buttonMap);
        extraButtonOptions.pin = -1;
        extraButtonOptions.enabled = false;
    }

    // convert DDI direction pin mapping to GPIO mapping config
    if (ddiOptions.enabled) {
        fromProtoBuf(ddiOptions.has_deprecatedUpPin,    &ddiOptions.deprecatedUpPin, GpioAction::BUTTON_PRESS_DDI_UP);
        fromProtoBuf(ddiOptions.has_deprecatedDownPin,  &ddiOptions.deprecatedDownPin, GpioAction::BUTTON_PRESS_DDI_DOWN);
        fromProtoBuf(ddiOptions.has_deprecatedLeftPin,  &ddiOptions.deprecatedLeftPin, GpioAction::BUTTON_PRESS_DDI_LEFT);
        fromProtoBuf(ddiOptions.has_deprecatedRightPin, &ddiOptions.deprecatedRightPin, GpioAction::BUTTON_PRESS_DDI_RIGHT);
    }

    // convert JS slider pin mappings to GPIO mapping config
    if (jsSliderOptions.enabled && isValidPin(jsSliderOptions.deprecatedPinSliderOne)) {
        switch (jsSliderOptions.deprecatedModeOne) {
            case DpadMode::DPAD_MODE_DIGITAL: {
                actions[jsSliderOptions.deprecatedPinSliderOne] = GpioAction::SUSTAIN_DP_MODE_DP; break;
            }
            case DpadMode::DPAD_MODE_LEFT_ANALOG: {
                actions[jsSliderOptions.deprecatedPinSliderOne] = GpioAction::SUSTAIN_DP_MODE_LS; break;
            }
            case DpadMode::DPAD_MODE_RIGHT_ANALOG: {
                actions[jsSliderOptions.deprecatedPinSliderOne] = GpioAction::SUSTAIN_DP_MODE_RS; break;
            }
            default: break;
        }
        jsSliderOptions.deprecatedPinSliderOne = -1;
    }
    if (jsSliderOptions.enabled && isValidPin(jsSliderOptions.deprecatedPinSliderTwo)) {
        switch (jsSliderOptions.deprecatedModeTwo) {
            case DpadMode::DPAD_MODE_DIGITAL: {
                actions[jsSliderOptions.deprecatedPinSliderTwo] = GpioAction::SUSTAIN_DP_MODE_DP; break;
            }
            case DpadMode::DPAD_MODE_LEFT_ANALOG: {
                actions[jsSliderOptions.deprecatedPinSliderTwo] = GpioAction::SUSTAIN_DP_MODE_LS; break;
            }
            case DpadMode::DPAD_MODE_RIGHT_ANALOG: {
                actions[jsSliderOptions.deprecatedPinSliderTwo] = GpioAction::SUSTAIN_DP_MODE_RS; break;
            }
            default: break;
        }
        jsSliderOptions.deprecatedPinSliderTwo = -1;
    }
    // convert SOCD slider pin mappings to GPIO mapping config
    if (socdSliderOptions.enabled && isValidPin(socdSliderOptions.deprecatedPinOne)) {
        switch (socdSliderOptions.deprecatedModeOne) {
            case SOCDMode::SOCD_MODE_UP_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinOne] = GpioAction::SUSTAIN_SOCD_MODE_UP_PRIO; break;
            }
            case SOCDMode::SOCD_MODE_NEUTRAL: {
                actions[socdSliderOptions.deprecatedPinOne] = GpioAction::SUSTAIN_SOCD_MODE_NEUTRAL; break;
            }
            case SOCDMode::SOCD_MODE_SECOND_INPUT_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinOne] = GpioAction::SUSTAIN_SOCD_MODE_SECOND_WIN; break;
            }
            case SOCDMode::SOCD_MODE_FIRST_INPUT_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinOne] = GpioAction::SUSTAIN_SOCD_MODE_FIRST_WIN; break;
            }
            case SOCDMode::SOCD_MODE_BYPASS: {
                actions[socdSliderOptions.deprecatedPinOne] = GpioAction::SUSTAIN_SOCD_MODE_BYPASS; break;
            }
            default: break;
        }
        socdSliderOptions.deprecatedPinOne = -1;
    }

    if (socdSliderOptions.enabled && isValidPin(socdSliderOptions.deprecatedPinTwo)) {
        switch (socdSliderOptions.deprecatedModeTwo) {
            case SOCDMode::SOCD_MODE_UP_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinTwo] = GpioAction::SUSTAIN_SOCD_MODE_UP_PRIO; break;
            }
            case SOCDMode::SOCD_MODE_NEUTRAL: {
                actions[socdSliderOptions.deprecatedPinTwo] = GpioAction::SUSTAIN_SOCD_MODE_NEUTRAL; break;
            }
            case SOCDMode::SOCD_MODE_SECOND_INPUT_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinTwo] = GpioAction::SUSTAIN_SOCD_MODE_SECOND_WIN; break;
            }
            case SOCDMode::SOCD_MODE_FIRST_INPUT_PRIORITY: {
                actions[socdSliderOptions.deprecatedPinTwo] = GpioAction::SUSTAIN_SOCD_MODE_FIRST_WIN; break;
            }
            case SOCDMode::SOCD_MODE_BYPASS: {
                actions[socdSliderOptions.deprecatedPinTwo] = GpioAction::SUSTAIN_SOCD_MODE_BYPASS; break;
            }
            default: break;
        }
        socdSliderOptions.deprecatedPinTwo = -1;
    }

    if (focusModeOptions.enabled) {
        fromProtoBuf(focusModeOptions.has_pin, &focusModeOptions.pin, GpioAction::SUSTAIN_FOCUS_MODE);
    }

    if (reverseOptions.enabled) {
        fromProtoBuf(reverseOptions.has_buttonPin, &reverseOptions.buttonPin, GpioAction::BUTTON_PRESS_INPUT_REVERSE);
    }

    // Assign all potential board config pins
    GpioAction boardConfig[NUM_BANK0_GPIOS] = {GPIO_PIN_00, GPIO_PIN_01, GPIO_PIN_02,
											   GPIO_PIN_03, GPIO_PIN_04, GPIO_PIN_05,
                                               GPIO_PIN_06, GPIO_PIN_07, GPIO_PIN_08,
                                               GPIO_PIN_09, GPIO_PIN_10, GPIO_PIN_11,
                                               GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14,
                                               GPIO_PIN_15, GPIO_PIN_16, GPIO_PIN_17,
                                               GPIO_PIN_18, GPIO_PIN_19, GPIO_PIN_20,
                                               GPIO_PIN_21, GPIO_PIN_22, GPIO_PIN_23,
                                               GPIO_PIN_24, GPIO_PIN_25, GPIO_PIN_26,
                                               GPIO_PIN_27, GPIO_PIN_28, GPIO_PIN_29,
#if NUM_BANK0_GPIOS > 32
                                               GPIO_PIN_30, GPIO_PIN_31, GPIO_PIN_32,
                                               GPIO_PIN_33, GPIO_PIN_34, GPIO_PIN_35,
                                               GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38,
                                               GPIO_PIN_39, GPIO_PIN_40, GPIO_PIN_41,
                                               GPIO_PIN_42, GPIO_PIN_43, GPIO_PIN_44,
                                               GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47,
#endif
                                               };

    // If we didn't import from protobuf, import from boardconfig
    for(unsigned int i = 0; i < NUM_BANK0_GPIOS; i++) {
        fromBoardConfig(i, boardConfig[i]);
    }

    // migrate I2C addons to use peripheral manager
    if (!peripheralOptions.blockI2C0.enabled && (
            (config.displayOptions.enabled && (config.displayOptions.deprecatedI2cBlock == 0)) ||
            (config.addonOptions.analogADS1219Options.enabled && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 0)) ||
            (config.addonOptions.wiiOptions.enabled && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 0))
        )
    ) {
        peripheralOptions.blockI2C0.enabled = (
            (config.displayOptions.enabled && (config.displayOptions.deprecatedI2cBlock == 0)) |
            (config.addonOptions.analogADS1219Options.enabled && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 0)) |
            (config.addonOptions.wiiOptions.enabled && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 0)) |
            (!!I2C0_ENABLED)
        );

        // pin configuration
        peripheralOptions.blockI2C0.sda = (
            isValidPin(config.displayOptions.deprecatedI2cSDAPin) && (config.displayOptions.deprecatedI2cBlock == 0) ?
            config.displayOptions.deprecatedI2cSDAPin :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSDAPin) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 0) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSDAPin :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSDAPin) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 0) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSDAPin :
                    I2C0_PIN_SDA
                )
            )
        );
        markAddonPinIfUsed(peripheralOptions.blockI2C0.sda);

        peripheralOptions.blockI2C0.scl = (
            isValidPin(config.displayOptions.deprecatedI2cSCLPin) && (config.displayOptions.deprecatedI2cBlock == 0) ?
            config.displayOptions.deprecatedI2cSCLPin :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSCLPin) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 0) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSCLPin :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSCLPin) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 0) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSCLPin :
                    I2C0_PIN_SCL
                )
            )
        );
        markAddonPinIfUsed(peripheralOptions.blockI2C0.scl);

        // option configuration
        peripheralOptions.blockI2C0.speed = (
            isValidPin(config.displayOptions.deprecatedI2cSpeed) && (config.displayOptions.deprecatedI2cBlock == 0) ?
            config.displayOptions.deprecatedI2cSpeed :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSpeed) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 0) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSpeed :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSpeed) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 0) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSpeed :
                    I2C0_SPEED
                )
            )
        );
    }

    if (!peripheralOptions.blockI2C1.enabled && (
            (config.displayOptions.enabled && (config.displayOptions.deprecatedI2cBlock == 1)) ||
            (config.addonOptions.analogADS1219Options.enabled && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 1)) ||
            (config.addonOptions.wiiOptions.enabled && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 1))
        )
    ) {
        peripheralOptions.blockI2C1.enabled = (
            (config.displayOptions.enabled && (config.displayOptions.deprecatedI2cBlock == 1)) |
            (config.addonOptions.analogADS1219Options.enabled && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 1)) |
            (config.addonOptions.wiiOptions.enabled && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 1)) |
            (!!I2C1_ENABLED)
        );

        // pin configuration
        peripheralOptions.blockI2C1.sda = (
            isValidPin(config.displayOptions.deprecatedI2cSDAPin) && (config.displayOptions.deprecatedI2cBlock == 1) ?
            config.displayOptions.deprecatedI2cSDAPin :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSDAPin) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 1) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSDAPin :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSDAPin) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 1) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSDAPin :
                    I2C1_PIN_SDA
                )
            )
        );
        markAddonPinIfUsed(peripheralOptions.blockI2C1.sda);

        peripheralOptions.blockI2C1.scl = (
            isValidPin(config.displayOptions.deprecatedI2cSCLPin) && (config.displayOptions.deprecatedI2cBlock == 1) ?
            config.displayOptions.deprecatedI2cSCLPin :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSCLPin) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 1) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSCLPin :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSCLPin) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 1) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSCLPin :
                    I2C1_PIN_SCL
                )
            )
        );
        markAddonPinIfUsed(peripheralOptions.blockI2C1.scl);

        // option configuration
        peripheralOptions.blockI2C1.speed = (
            isValidPin(config.displayOptions.deprecatedI2cSpeed) && (config.displayOptions.deprecatedI2cBlock == 1) ?
            config.displayOptions.deprecatedI2cSpeed :
            (
                isValidPin(config.addonOptions.analogADS1219Options.deprecatedI2cSpeed) && (config.addonOptions.analogADS1219Options.deprecatedI2cBlock == 1) ?
                config.addonOptions.analogADS1219Options.deprecatedI2cSpeed :
                (
                    isValidPin(config.addonOptions.wiiOptions.deprecatedI2cSpeed) && (config.addonOptions.wiiOptions.deprecatedI2cBlock == 1) ?
                    config.addonOptions.wiiOptions.deprecatedI2cSpeed :
                    I2C1_SPEED
                )
            )
        );
    }

    // migrate USB addons to use peripheral manager
    if (!peripheralOptions.blockUSB0.enabled && (keyboardHostOptions.enabled || psPassthroughOptions.enabled)) {
        peripheralOptions.blockUSB0.enabled = keyboardHostOptions.enabled | psPassthroughOptions.enabled | false;

        if (peripheralOptions.blockUSB0.enabled) {
            peripheralOptions.blockUSB0.enable5v = (
                isValidPin(keyboardHostOptions.deprecatedPin5V) ?
                keyboardHostOptions.deprecatedPin5V :
                (
                    isValidPin(psPassthroughOptions.deprecatedPin5V) ?
                    psPassthroughOptions.deprecatedPin5V :
                    -1
                )
            );
            markAddonPinIfUsed(peripheralOptions.blockUSB0.enable5v);

            peripheralOptions.blockUSB0.dp = (
                isValidPin(keyboardHostOptions.deprecatedPinDplus) ?
                keyboardHostOptions.deprecatedPinDplus :
                (
                    isValidPin(psPassthroughOptions.deprecatedPinDplus) ?
                    psPassthroughOptions.deprecatedPinDplus :
                    -1
                )
            );
            markAddonPinIfUsed(peripheralOptions.blockUSB0.dp);
            if (isValidPin(peripheralOptions.blockUSB0.dp))
                actions[peripheralOptions.blockUSB0.dp+1] = GpioAction::ASSIGNED_TO_ADDON;
        }
    }

    markAddonPinIfUsed(config.ledOptions.dataPin);
    // check if PLED PINs are actually GPIOs or not
    // pledPin used to be used for RGB indexes, so we should only mark the GPIO
    // as assigned to addon if in PWM mode
    if (config.ledOptions.pledType == PLEDType::PLED_TYPE_PWM)
    {
        // fields are being used for PWM, so they are GPIOs; reserve them
        markAddonPinIfUsed(config.ledOptions.pledPin1);
        markAddonPinIfUsed(config.ledOptions.pledPin2);
        markAddonPinIfUsed(config.ledOptions.pledPin3);
        markAddonPinIfUsed(config.ledOptions.pledPin4);
    }

    for (Pin_t pin = 0; pin < (Pin_t)NUM_BANK0_GPIOS; pin++) {
        config.gpioMappings.pins[pin].action = actions[pin];
    }
    // reminder that this must be set or else nanopb won't retain anything
    config.gpioMappings.pins_count = NUM_BANK0_GPIOS;

    config.migrations.gpioMappingsMigrated = true;
}

// populate existing configurations' buttonsMask and auxMask to mirror behavior
// from the behavior before this code merged. totally new configs get their
// board defaults via initUnsetPropertiesWithDefaults
void hotkeysMigration(Config& config)
{
    HotkeyOptions& hotkeys = config.hotkeyOptions;

    // if dpadMask is defined and buttonsMask and auxMask aren't, then this
    // hotkey was saved at least once in the past when the button shortcut was
    // known as F1/F2, so they should be made to reflect the
    // previously-hardcoded values for those hotkeys

    // F1 == S1 | S2, no Fn
    if (hotkeys.hotkey01.has_dpadMask && (!hotkeys.hotkey01.has_auxMask || !hotkeys.hotkey01.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey01, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey01, buttonsMask, GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
    }
    if (hotkeys.hotkey02.has_dpadMask && (!hotkeys.hotkey02.has_auxMask || !hotkeys.hotkey02.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey02, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey02, buttonsMask, GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
    }
    if (hotkeys.hotkey03.has_dpadMask && (!hotkeys.hotkey03.has_auxMask || !hotkeys.hotkey03.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey03, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey03, buttonsMask, GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
    }
    if (hotkeys.hotkey04.has_dpadMask && (!hotkeys.hotkey04.has_auxMask || !hotkeys.hotkey04.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey04, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey04, buttonsMask, GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
    }

    // F2 == S2 | A1, no Fn
    if (hotkeys.hotkey05.has_dpadMask && (!hotkeys.hotkey05.has_auxMask || !hotkeys.hotkey05.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey05, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey05, buttonsMask, GAMEPAD_MASK_S2 | GAMEPAD_MASK_A1);
    }
    if (hotkeys.hotkey06.has_dpadMask && (!hotkeys.hotkey06.has_auxMask || !hotkeys.hotkey06.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey06, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey06, buttonsMask, GAMEPAD_MASK_S2 | GAMEPAD_MASK_A1);
    }
    if (hotkeys.hotkey07.has_dpadMask && (!hotkeys.hotkey07.has_auxMask || !hotkeys.hotkey07.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey07, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey07, buttonsMask, GAMEPAD_MASK_S2 | GAMEPAD_MASK_A1);
    }
    if (hotkeys.hotkey08.has_dpadMask && (!hotkeys.hotkey08.has_auxMask || !hotkeys.hotkey08.has_buttonsMask)) {
	INIT_UNSET_PROPERTY(hotkeys.hotkey08, auxMask, 0);
	INIT_UNSET_PROPERTY(hotkeys.hotkey08, buttonsMask, GAMEPAD_MASK_S2 | GAMEPAD_MASK_A1);
    }

    config.migrations.hotkeysMigrated = true;
}

// -----------------------------------------------------
// Loading / Saving
// -----------------------------------------------------

// We put a ConfigFooter struct at the end of the flash area reserved for FlashPROM. It contains a magicvalue, the size
// of the serialized config data and a CRC of that data. This information allows us to both locate and verify the stored
// data. The serialized data is located directly before the footer:
//
//                       FlashPROM block
// ┌────────────────────────────┴─────────────────────────────┐
// ┌──────────────┬────────────────────────────────────┬──────┐
// │Unused memory │Protobuf data                       │Footer│
// └──────────────┴────────────────────────────────────┴──────┘
//
struct ConfigFooter
{
    uint32_t dataSize;
    uint32_t dataCrc;
    uint32_t magic;

    bool operator==(const ConfigFooter& other) const
    {
        return
            dataSize == other.dataSize &&
            dataCrc == other.dataCrc &&
            magic == other.magic;
    }
};

static const uint32_t FOOTER_MAGIC = 0xd2f1e365;

// Verify that the maximum size of the serialized Config object fits into the allocated flash block
#if defined(Config_size)
    static_assert(Config_size + sizeof(ConfigFooter) <= EEPROM_SIZE_BYTES, "Maximum size of Config exceeds the maximum size allocated for FlashPROM");
#else
    #error "Maximum size of Config cannot be determined statically, make sure that you do not use any dynamically sized arrays or strings"
#endif

static bool loadConfigInner(Config& config)
{
    config = Config Config_init_zero;

    const uint8_t* flashEnd = reinterpret_cast<const uint8_t*>(EEPROM_ADDRESS_START) + EEPROM_SIZE_BYTES;
    ConfigFooter footer;
    memcpy(&footer, flashEnd - sizeof(ConfigFooter), sizeof(footer));

    // Check for presence of magic value
    if (footer.magic != FOOTER_MAGIC)
    {
        return false;
    }

        // Check if dataSize exceeds the reserved space
    if (footer.dataSize + sizeof(ConfigFooter) > EEPROM_SIZE_BYTES)
    {
        return false;
    }

    const uint8_t* dataPtr = flashEnd - sizeof(ConfigFooter) - footer.dataSize;

    // Verify CRC32 hash
    if (CRC32::calculate(dataPtr, footer.dataSize) != footer.dataCrc)
    {
        return false;
    }

    // We are now sufficiently confident that the data is valid so we run the deserialization
    pb_istream_t inputStream = pb_istream_from_buffer(dataPtr, footer.dataSize);
    return pb_decode(&inputStream, Config_fields, &config);
}

void ConfigUtils::load(Config& config)
{
    const bool loaded = loadConfigInner(config);

    if (!loaded)
    {
        // We could neither deserialize Protobuf config data nor legacy config data.
        // We are probably dealing with a new device and therefore initialize the config to default values.
        config = Config Config_init_default;
    }

    // run migrations
    if (!config.migrations.hotkeysMigrated)
        hotkeysMigration(config);

    // Make sure that fields that were not deserialized are properly initialized.
    // They were probably added with a newer version of the firmware.
    initUnsetPropertiesWithDefaults(config);

    // Run migrations that need to happen after initUnset...
    // ProtoBuf && Board Config settings are loaded here
    if (!config.migrations.gpioMappingsMigrated)
        gpioMappingsMigrationCore(config);

    // Update boardVersion, in case we migrated from an older version
    strncpy(config.boardVersion, GP2040VERSION, sizeof(config.boardVersion));
    config.boardVersion[sizeof(config.boardVersion) - 1] = '\0';
    config.has_boardVersion = true;

    // Save, to make sure we persist any performed migration steps
    save(config);
}

static void setHasFlags(const pb_msgdesc_t* fields, void* s)
{
    pb_field_iter_t iter;
    if (!pb_field_iter_begin(&iter, fields, s))
    {
        return;
    }

    do
    {
        // Not implemented for extension fields
        assert(PB_LTYPE(iter.type) != PB_LTYPE_EXTENSION);

        switch (PB_HTYPE(iter.type))
        {
            case PB_HTYPE_OPTIONAL:
            {
                *reinterpret_cast<bool*>(iter.pSize) = true;

                // Recurse into sub-messages
                if (PB_LTYPE(iter.type) == PB_LTYPE_SUBMESSAGE)
                {
                    assert(iter.submsg_desc);
                    assert(iter.pData);

                    setHasFlags(iter.submsg_desc, iter.pData);
                }
                break;
            }

            case PB_HTYPE_REPEATED:
            {
                // Recurse into sub-messages
                if (PB_LTYPE(iter.type) == PB_LTYPE_SUBMESSAGE)
                {
                    assert(iter.submsg_desc);
                    assert(iter.pData);
                    assert(iter.pSize);

                    const pb_size_t array_size = *reinterpret_cast<pb_size_t*>(iter.pSize);
                    pb_byte_t* item_ptr = reinterpret_cast<pb_byte_t*>(iter.pData);
                    for (pb_size_t index = 0; index < array_size; ++index)
                    {
                        setHasFlags(iter.submsg_desc, item_ptr);
                        item_ptr += iter.data_size;
                    }
                }
                break;
            }

            default:
                // We do not support any other htypes of fields
                assert(false);
                continue;
        }
    } while (pb_field_iter_next(&iter));
}

bool ConfigUtils::save(Config& config)
{
    // We only allow saves from core0. Saves from core1 have to be marshalled to core0.
    assert(get_core_num() == 0);
    if (get_core_num() != 0)
    {
        return false;
    }

    // Set all has_XXX flags to true, we want to save all fields.
    // If we didn't do this we would have to remember to set the has_XXX flag manually whenever we change a field from
    // its default value.
    setHasFlags(Config_fields, &config);

    // Encode the data directly into the cache of FlashPROM
    pb_ostream_t outputStream = pb_ostream_from_buffer(EEPROM.writeCache, EEPROM_SIZE_BYTES - sizeof(ConfigFooter));
    const bool encoded = pb_encode(&outputStream, Config_fields, &config);
    if (!encoded)
    {
        return false;
    }

    // Create the new footer
    ConfigFooter newFooter;
    newFooter.dataSize = outputStream.bytes_written;
    newFooter.dataCrc = CRC32::calculate(EEPROM.writeCache, newFooter.dataSize);
    newFooter.magic = FOOTER_MAGIC;

    // The data has changed when the footer content has changed. Only then do we acutally need to save.
    ConfigFooter oldFooter;
    memcpy(&oldFooter, EEPROM.writeCache + EEPROM_SIZE_BYTES - sizeof(ConfigFooter), sizeof(oldFooter));
    if (newFooter == oldFooter)
    {
        // The data has not changed, no saving neccessary.
        return true;
    }

    // Write the footer
    memcpy(EEPROM.writeCache + EEPROM_SIZE_BYTES - sizeof(ConfigFooter), &newFooter, sizeof(newFooter));

    // Move the encoded data in memory down to the footer
    memmove(EEPROM.writeCache + EEPROM_SIZE_BYTES - sizeof(ConfigFooter) - newFooter.dataSize, EEPROM.writeCache, newFooter.dataSize);
    memset(EEPROM.writeCache, 0, EEPROM_SIZE_BYTES - sizeof(ConfigFooter) - newFooter.dataSize);

    EEPROM.commit();

    return true;
}
