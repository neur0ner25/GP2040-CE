// GP2040 includes
#include "gp2040aux.h"
#include "gamepad.h"

#include "NeoPico.h"
#include "pico/platform.h"
#include "pico/time.h"

GP2040Aux::GP2040Aux() : inputDriver(nullptr), isReady(false) {
}

GP2040Aux::~GP2040Aux() {
}

static void initOnboardLed() {
	NeoPico neopico;
	neopico.Setup(16, 1, LED_FORMAT_GRB, pio1, 0);
	uint32_t frame[FRAME_MAX] = { (5 << 8) }; // Static red at brightness 5
	neopico.SetFrame(frame);
	neopico.Show();
}

void GP2040Aux::setup() {
	// 1. Static RED LED on GP16
	initOnboardLed();

	// Ready to sync Core0 and Core1
	isReady = true;
}

void GP2040Aux::run() {
	while (1) {
		sleep_ms(50);
	}
}
