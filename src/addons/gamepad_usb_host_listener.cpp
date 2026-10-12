#include "addons/gamepad_usb_host_listener.h"
#include "storagemanager.h"
#include "class/hid/hid.h"
#include "class/hid/hid_host.h"

#include "hosts/Xbox360Host.h"

void GamepadUSBHostListener::setup() {
    _controller_host = nullptr;
    _controller_dev_addr = 0;
    _controller_instance = 0;
    _last_update = getMillis() - GAMEPAD_HOST_POLL_INTERVAL_MS - 1;
}

void GamepadUSBHostListener::update() {
    if ( _controller_host == nullptr ) return;

    // Controller host updates (rumble, LEDs, etc. outside of gamepad input reports)
    if (getMillis() - _last_update > GAMEPAD_HOST_POLL_INTERVAL_MS) {
        _controller_host->update();
        _last_update = getMillis();
    }
    
    // Controller host driver changes gamepad
    Gamepad *gamepad = Storage::getInstance().GetGamepad();
    _controller_host->gamepad(gamepad);    
}

void GamepadUSBHostListener::mount(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    // We only host XInput devices (Xbox360Host) for the 8BitDo wireless receiver
}

void GamepadUSBHostListener::xmount(uint8_t dev_addr, uint8_t instance, uint8_t controllerType, uint8_t subtype) {
    // Only host one controller at a time
    if ( _controller_host != nullptr ) return;

    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    _controller_host = nullptr;
    if ( Xbox360Host::match(dev_addr, instance, vid, pid, controllerType) ) {
        _controller_host = new Xbox360Host();
    }

    if ( _controller_host != nullptr ) {
        _controller_dev_addr = dev_addr;
        _controller_instance = instance;
        _controller_host->initialize(dev_addr, instance, vid, pid, nullptr, 0);
    }
}

void GamepadUSBHostListener::unmount(uint8_t dev_addr) {
    if ( _controller_host == nullptr || _controller_dev_addr != dev_addr ) return;

    _controller_host->shutdown();
    delete _controller_host;
    _controller_host = nullptr;
}

void GamepadUSBHostListener::report_received(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    if ( _controller_host != nullptr && 
        _controller_dev_addr == dev_addr &&
        _controller_instance == instance ) {
        _controller_host->process(report, len);
    } 
}

void GamepadUSBHostListener::set_report_complete(uint8_t dev_addr, uint8_t instance, uint8_t report_id, uint8_t report_type, uint16_t len) {
    if ( _controller_host != nullptr &&
        _controller_dev_addr == dev_addr &&
        _controller_instance == instance ) {
        _controller_host->set_report_complete(dev_addr, instance, report_id, report_type, len);
    } 
}

void GamepadUSBHostListener::get_report_complete(uint8_t dev_addr, uint8_t instance, uint8_t report_id, uint8_t report_type, uint16_t len) {
    if ( _controller_host != nullptr &&
        _controller_dev_addr == dev_addr &&
        _controller_instance == instance ) {
        _controller_host->get_report_complete(dev_addr, instance, report_id, report_type, len);
    } 
}
