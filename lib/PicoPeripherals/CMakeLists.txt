add_library(PicoPeripherals
    peripheral_usb.cpp
)
target_include_directories(PicoPeripherals PRIVATE .)
target_include_directories(PicoPeripherals INTERFACE .)
target_link_libraries(PicoPeripherals 
    pico_stdlib
    hardware_gpio
    tinyusb_pico_pio_usb
)
