# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

if(DEFINED ENV{PICO_BOARD})
  set(PICO_BOARD $ENV{PICO_BOARD} CACHE STRING "Board type")
else()
  set(PICO_BOARD wavesharezero CACHE STRING "Board type")
endif()

if(DEFINED ENV{PICO_PLATFORM})
  set(PICO_PLATFORM $ENV{PICO_PLATFORM} CACHE STRING "Board platform")
else()
  set(PICO_PLATFORM rp2040 CACHE STRING "Board platform")
endif()

if(DEFINED ENV{GP2040_BOARDCONFIG})
  set(GP2040_BOARDCONFIG $ENV{GP2040_BOARDCONFIG})
elseif(NOT DEFINED GP2040_BOARDCONFIG)
  set(GP2040_BOARDCONFIG WaveshareZeroDongle)
endif()

# check for existing cmake files in the event of changing PICO_BOARD or PICO_PLATFORM
if (GP2040_BOARDCONFIG)
  message("ProBridge-S2 board config is set: ${GP2040_BOARDCONFIG}")
  set(GP2040_BOARD_CONFIG_FILE "${CMAKE_CURRENT_LIST_DIR}/configs/${GP2040_BOARDCONFIG}/${GP2040_BOARDCONFIG}.cmake")

  if (EXISTS "${GP2040_BOARD_CONFIG_FILE}")
    message(STATUS "Using board config file: ${GP2040_BOARD_CONFIG_FILE}")
    set(PICO_BOARD_HEADER_DIRS "${CMAKE_CURRENT_LIST_DIR}/configs/${GP2040_BOARDCONFIG}")
    include("${GP2040_BOARD_CONFIG_FILE}")
  endif()
endif()

cmake_minimum_required(VERSION 3.10...4.0)
include(CMakePrintHelpers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.3.1")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.3.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# ProBridge-S2 version
set(PROBRIDGE_VERSION "1.0.0")

# set the build id based on git
find_package(Git)
execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=7
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_REPO_BUILD_ID
	OUTPUT_STRIP_TRAILING_WHITESPACE)
set(GIT_REPO_VERSION "v${PROBRIDGE_VERSION}")
set(CMAKE_GIT_REPO_VERSION "${PROBRIDGE_VERSION}")
configure_file("headers/version.h.in" "headers/version.h")

project(ProBridge-S2 LANGUAGES C CXX ASM VERSION ${PROBRIDGE_VERSION})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/configs/${GP2040_BOARDCONFIG})

if(DEFINED ENV{PICO_PIO_USB_PATH})
  message(STATUS "Found custom Pico-PIO-USB path, using it.")
  set(PICO_PIO_USB_PATH $ENV{PICO_PIO_USB_PATH})
elseif(NOT DEFINED PICO_PIO_USB_PATH)
  message(STATUS "Using default Pico-PIO-USB.")
  set(PICO_PIO_USB_PATH ${CMAKE_SOURCE_DIR}/lib/pico_pio_usb)
endif()

add_compile_options(-Wall
        -Wtype-limits
        -Wno-format
        -Wno-unused-function
        )

include(compile_proto.cmake)
compile_proto()

# pull in tinyUSB
set(PICO_TINYUSB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/tinyusb")
set(PICO_USE_FASTEST_SUPPORTED_CLOCK 1)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Remove unused code and data
add_compile_options(-fdata-sections -ffunction-sections)
add_link_options(-Wl,--gc-sections)

add_subdirectory(lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fstack-usage -Wstack-usage=500 -Wnull-dereference -Wcast-align -Wimplicit-fallthrough)
  add_compile_definitions(PICO_USE_STACK_GUARDS=1)
endif()

add_compile_definitions(PICO_STACK_SIZE=0x1000)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/gp2040.cpp
  src/gp2040aux.cpp
  src/gamepad.cpp
  src/gamepad/GamepadState.cpp
  src/addonmanager.cpp
  src/drivers/shared/xinput_host.cpp
  src/drivers/switch2pro/Switch2ProDriver.cpp
  src/drivermanager.cpp
  src/peripheralmanager.cpp
  src/storagemanager.cpp
  src/system.cpp
  src/usbdriver.cpp
  src/usbhostmanager.cpp
  src/config_utils.cpp
  src/addons/board_led.cpp
  src/addons/bootsel_button.cpp
  src/addons/neopicoleds.cpp
  src/addons/gamepad_usb_host.cpp
  src/addons/gamepad_usb_host_listener.cpp
  src/hosts/Xbox360Host.cpp
  ${PROTO_OUTPUT_DIR}/enums.pb.c
  ${PROTO_OUTPUT_DIR}/config.pb.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

pico_set_program_name(${PROJECT_NAME} "ProBridge-S2")
pico_set_program_version(${PROJECT_NAME} "${PROBRIDGE_VERSION}")

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  pico_bootsel_via_double_reset
  tinyusb_device
  tinyusb_host
  tinyusb_pico_pio_usb
  CRC32
  FlashPROM
  NeoPico
  hardware_adc
  hardware_pwm
  PicoPeripherals
  pico_mbedtls
  nanopb
)

target_include_directories(${PROJECT_NAME} PUBLIC
  headers
  headers/addons
  headers/configs
  headers/drivers
  headers/drivers/shared
  headers/events
  headers/gamepad
  headers/hosts
  configs/${GP2040_BOARDCONFIG}
  ${PROTO_OUTPUT_DIR}
  ${CMAKE_BINARY_DIR}/headers
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
  BOARD_CONFIG_FILE_NAME="$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>"
  GP2040_BOARDCONFIG="${GP2040_BOARDCONFIG}"
)

if(PICO_RP2040)
  target_compile_definitions(${PROJECT_NAME} PUBLIC PICO_FLOAT_IN_RAM=1)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(${PROJECT_NAME})

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.uf2
  ${CMAKE_CURRENT_LIST_DIR}/README.md
  DESTINATION .
)
