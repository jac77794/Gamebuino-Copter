// corridor generation

/*
config
*/

int game_speed = 4;

float size_max = 44;
float size_min = 12;

int reduce_next = 30; // fixed rate of reduction for fairness in repeat plays
float reduce_mult = 0.9;

float yv_granularity = 100;
float yv_change_range = 2;
float yv_change_next_range[2] = {4,12};

int gate_size_max = 20;
int gate_size_min = 9;
float gate_length_range[2] = {2,5};
float gate_next_range[2] = {5,15};
int gate_first = 60;

bool debug = false;

/*
runtime
*/

CorridorPart corridor[21]; // length derived from (LCDWIDTH / game_speed), update if game_speed changed

int progress;
float last_h;
float last_y;

float yv;
int yv_change_next;

int gate_next;
int gate_size;
int gate_length;

int max_parts;
int last_part;

void init_corridor() {
	max_parts = LCDWIDTH/game_speed;
	last_part = max_parts-1;

	// reset runtime vars
	progress = 0;
	last_h = size_max;
	last_y = LCDHEIGHT/2;
	yv = 0;
	yv_change_next = game_speed*reduce_next;
	gate_next = gate_first;

	// reset corridor parts
	for (int i = 0; i < max_parts; i++) {
		CorridorPart c;
		c.h = last_h;
		c.y = last_y;
		corridor[i] = c;
	}
}

void update_corridor() {
	progress++;
	randomSeed(progress*micros());

	for (int i = 0; i < last_part; i++) {
		corridor[i] = corridor[i+1];
	}

	float new_h = last_h;
	float new_y = last_y;

	// narrow gradually (ignore gates)
	if (progress % (game_speed*reduce_next) == 0 && new_h > size_min) {
		new_h = max(new_h*reduce_mult, size_min);
		last_h = new_h;
	}

	if (progress >= gate_next) {
		// generate a gate
		gate_next += game_speed*range_random(gate_next_range);
		gate_length = range_random(gate_length_range);

		float this_gate_size_max = min(new_h, gate_size_max);
		new_h = random(gate_size_min, this_gate_size_max);

		float y_top = last_y-(last_h/2);
		float y_bot = last_y+(last_h/2);
		new_y = random(y_top+(new_h/2), y_bot-(new_h/2));
	}
	else if (gate_length > 0) {
		// gate remains fixed
		new_h = corridor[last_part].h;
		new_y = corridor[last_part].y;
		gate_length--;
	}
	else {
		// no gate, move corridor as normal
		if (progress >= yv_change_next) {
			yv_change_next += game_speed*range_random(yv_change_next_range)*game_speed;
			yv = (((yv_granularity/2) - random(yv_granularity)) / yv_granularity) * yv_change_range;
		}

		new_y += yv;

		float y_top = new_y-(new_h/2);
		float y_bot = new_y+(new_h/2);
		if (y_top < 0) {
			new_y = new_y-y_top;
			yv = -yv;
		}
		if (y_bot > LCDHEIGHT) {
			new_y = new_y-(y_bot-LCDHEIGHT);
			yv = -yv;
		}

		last_y = new_y;
	}

	CorridorPart new_c;
	new_c.h = new_h;
	new_c.y = new_y;
	corridor[last_part] = new_c;
}

void draw_corridor() {
	for (int i = 0; i < max_parts; i++) {
		CorridorPart c = corridor[i];
		int x = i*game_speed;
		int y_top = c.y-(c.h/2);
		int y_bot = c.y+(c.h/2);
		gb.display.fillRect(x, 0, game_speed, y_top);
		gb.display.fillRect(x, y_bot, game_speed, LCDHEIGHT-y_bot);
	}
}

extern Helicopter heli;

bool did_crash() {
	if (debug) {
		return false;
	}

	// don't really need to collision test beyond heli.x, room for optimisation
	for (int i = 0; i < max_parts; i++) {
		CorridorPart c = corridor[i];
		int x = i*game_speed;
		int y_top = c.y-(c.h/2);
		int y_bot = c.y+(c.h/2);
		if (gb.collideRectRect(heli.x, heli.y, heli.size, heli.size, x, 0, game_speed, y_top) ||
			gb.collideRectRect(heli.x, heli.y, heli.size, heli.size, x, y_bot, game_speed, LCDHEIGHT-y_bot)) {
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
	gb.display.print(progress);
}
