#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

#include "Copter.h"

extern const byte font5x7[];

void setup() {
	init_corridor();
	init_heli();
	gb.begin();
	gb.setFrameRate(20);
	gb.display.setFont(font5x7);
	gb.titleScreen(F("Copter by Anny"));
}

void loop() {
	if (gb.update()) {
		if (gb.buttons.pressed(BTN_C)) {
			init_corridor();
			init_heli();
			gb.display.setFont(font5x7);
			gb.titleScreen(F("Copter by Anny"));
		}

		if (! did_crash()) {
			update_corridor();
			update_heli();
		}

		draw_corridor();
		draw_heli();
		draw_score();
	}
}
