#ifndef _NEOPICOLEDS_H_
#define _NEOPICOLEDS_H_

#include "gpaddon.h"
#include "NeoPico.h"

#ifndef BOARD_LEDS_PIN
#define BOARD_LEDS_PIN 16
#endif

class NeoPicoLEDAddon : public GPAddon {
public:
    virtual bool available();
    virtual void setup();
    virtual void process();
    virtual void reinit() {}
    virtual void preprocess() {}
    virtual void postprocess(bool reportSent) {}
    virtual std::string name() { return "NeoPicoLEDAddon"; }
private:
    NeoPico neopico;
    bool isSetup = false;
};

#endif
