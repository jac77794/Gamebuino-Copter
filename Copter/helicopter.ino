// helicopter (player) object

float rise_rate = 0.3;
float rise_max = 2;
float fall_rate = 0.4;
float fall_max = -2.5;

Helicopter heli;

void init_heli() {
	heli.size = 3;
	heli.x = 4;
	heli.y = LCDHEIGHT/2;
	heli.yv = 0;
}

void update_heli() {
	if (gb.buttons.repeat(BTN_A,1) || gb.buttons.repeat(BTN_B,1)) {
		heli.yv = heli.yv+rise_rate;
		if (heli.yv > rise_max) {
			heli.yv = rise_max;
		}
	}
	else {
		heli.yv = heli.yv-fall_rate;
		if (heli.yv < fall_max) {
			heli.yv = fall_max;
		}
	}

	heli.y = heli.y-heli.yv;
	if ((heli.y+heli.size) > LCDHEIGHT) {
		heli.y = LCDHEIGHT-heli.size;
	}
	else if (heli.y < 0) {
		heli.y = 0;
	}
}

void draw_heli() {
	gb.display.fillRect(heli.x, (int)heli.y, heli.size, heli.size);
}
