// additional typedefs and methods used across Copter scripts

typedef struct {
	float h;
	float y;
} CorridorPart;

typedef struct {
	int size;
	int x;
	float y;
	float yv;
} Helicopter;

float range_random(float* r) {
	return random(r[0], r[1]);
}
