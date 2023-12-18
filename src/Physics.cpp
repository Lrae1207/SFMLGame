#include "Physics.hpp"

double phys::distance2D(double x1, double y1, double x2, double y2) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	float d = sqrtf(dx*dx + dy*dy);
	return d;
}
double phys::calculateGravityAccel(double x1, double y1, double x2, double y2, double mass2) {
	return GGRAM * GRAVITY_CONSTANT * mass2 / distance2D(x1, y1, x2, y2);
}