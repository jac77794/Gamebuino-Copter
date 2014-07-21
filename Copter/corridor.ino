int corridor_part_width = 4;
int corridor_size_init = 44;
int corridor_size = corridor_size_init*1;
int corridor_size_min = 12;
int corridor_max_parts = 21; // derived from LCDWIDTH/corridor_part_width
int corridor_progress = 0;

int corridor_reduce_rate = 120;
float corridor_reduce_mult = 0.9;
int corridor_gate_length[2] = {2,5};
int corridor_gate_freq[2] = {20,60};
int corridor_gate_size_max = 20;
int corridor_gate_size_min = 9;
int corridor_gate_do_next = 100;
int corridor_gate_size_now = 25;
int corridor_gate_length_now = 2;

// arrays have length=corridor_max_parts
float corridor_h[21];
float corridor_y[21];

float corridor_yv = 0;
float corridor_yv_gran = 100;
float corridor_yv_range = 2;
float corridor_yv_dirchangerate[2] = {20,60};
float corridor_yv_dirchange_do_next = 100;

void init_corridor() {
	corridor_progress = 0;
	corridor_size = corridor_size_init;
	corridor_gate_do_next = 100;
	corridor_yv = 0;
	corridor_yv_dirchange_do_next = 100;

	int def_y = (LCDHEIGHT-corridor_size)/2;
	for (int i = 0; i < corridor_max_parts; i++) {
		corridor_h[i] = corridor_size;
		corridor_y[i] = def_y;
	}
}

void update_corridor() {
	corridor_progress++;
	randomSeed(corridor_progress*micros());

	for (int i = 0; i < corridor_max_parts-1; i++) {
		corridor_h[i] = corridor_h[i+1];
		corridor_y[i] = corridor_y[i+1];
	}

	// narrow gradually
	if (corridor_progress % corridor_reduce_rate == 0 && corridor_reduce_rate > corridor_size_min) {
		corridor_size = corridor_size*corridor_reduce_mult;
		if (corridor_size < corridor_size_min) {
			corridor_size = corridor_size_min;
		}
	}

	// change direction, speed of corridor
	if (corridor_progress >= corridor_yv_dirchange_do_next) {
		corridor_yv_dirchange_do_next += random(corridor_yv_dirchangerate[0], corridor_yv_dirchangerate[1]);
		corridor_yv = ((random(corridor_yv_gran)-(corridor_yv_gran/2))/corridor_yv_gran)*corridor_yv_range;
	}

	// generate gate and randomise next
	int last_y = corridor_y[corridor_max_parts-2];
	int last_max_y = last_y+corridor_size;
	if (corridor_progress >= corridor_gate_do_next) {
		corridor_gate_do_next += random(corridor_gate_freq[0], corridor_gate_freq[1]);
		int corridor_this_gate_size_max = corridor_size < corridor_gate_size_max ? corridor_size : corridor_gate_size_max;
		corridor_gate_size_now = random(corridor_gate_size_min, corridor_this_gate_size_max);
		corridor_gate_length_now = random(corridor_gate_length[0], corridor_gate_length[1]);
		corridor_y[corridor_max_parts-1] = random(last_y, last_max_y-corridor_gate_size_now);
	}

	// finalise values
	if (corridor_gate_length_now > 0) {
		corridor_h[corridor_max_parts-1] = corridor_gate_size_now;
		corridor_gate_length_now--;
	}
	else {
		corridor_h[corridor_max_parts-1] = corridor_size;
		corridor_y[corridor_max_parts-1] += corridor_yv;
	}

	// reverse direction if hitting a wall
	if (corridor_y[corridor_max_parts-1] < 0) {
		corridor_y[corridor_max_parts-1] = 0;
		corridor_yv = -corridor_yv;
	}
	if (corridor_y[corridor_max_parts-1]+corridor_size > LCDHEIGHT) {
		corridor_y[corridor_max_parts-1] = LCDHEIGHT-corridor_size;
		corridor_yv = -corridor_yv;
	}
}

void draw_corridor() {
	for (int l = 0; l < corridor_max_parts; l++) {
		int x = l*corridor_part_width;
		int y = corridor_y[l];
		int h = corridor_h[l];
		gb.display.fillRect(x, 0, corridor_part_width, y);
		gb.display.fillRect(x, y+h, corridor_part_width, LCDHEIGHT-(y+h));
	}
}

extern int heli_x;
extern float heli_y;
extern int heli_size;

bool did_crash() {
	for (int l = 0; l < corridor_max_parts; l++) {
		int x = l*corridor_part_width;
		int y = corridor_y[l];
		int h = corridor_h[l];
		if (gb.collideRectRect(heli_x, heli_y, heli_size, heli_size, x, 0, corridor_part_width, y) ||
			gb.collideRectRect(heli_x, heli_y, heli_size, heli_size, x, y+h, corridor_part_width, LCDHEIGHT-(y+h))) {
				return true;
		}
	}
	return false;
}

extern const byte font3x5[];
extern const byte font5x7[];

void draw_score() {
	if (did_crash()) {
		gb.display.setFont(font5x7);
	}
	else {
		gb.display.setFont(font3x5);
	}
	gb.display.print(corridor_progress);
}
