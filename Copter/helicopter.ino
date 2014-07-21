float heli_rise_rate = 0.3;
float heli_rise_maxvel = 2;
float heli_fall_rate = 0.4;
float heli_fall_maxvel = -2.5;
float heli_vel = 0;
int heli_x = 5;
float heli_y = LCDHEIGHT/2;
int heli_size = 3;

void init_heli() {
	heli_vel = 0;
	heli_y = LCDHEIGHT/2;
}

void update_heli() {
	if (gb.buttons.repeat(BTN_A,1) || gb.buttons.repeat(BTN_B,1)) {
		heli_vel = heli_vel+heli_rise_rate;
		if (heli_vel > heli_rise_maxvel) {
			heli_vel = heli_rise_maxvel;
		}
	}
	else {
		heli_vel = heli_vel-heli_fall_rate;
		if (heli_vel < heli_fall_maxvel) {
			heli_vel = heli_fall_maxvel;
		}
	}

	heli_y = heli_y-heli_vel;
	if ((heli_y+heli_size) > LCDHEIGHT) {
		heli_y = LCDHEIGHT-heli_size;
	}
	else if (heli_y < 0) {
		heli_y = 0;
	}
}

void draw_heli() {
	gb.display.fillRect(heli_x, (int)heli_y, heli_size, heli_size);
}
