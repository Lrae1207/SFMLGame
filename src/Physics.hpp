#pragma once

#include <cmath>

namespace phys {
	const double PI = 3.1415926535;
	const double RADTODEG = 180.0f / PI;
	const double DEGTORAD = PI/180.0f;
	const double GRAVITY_CONSTANT = 6.6743f * pow(10, -11);
	const double GGRAM = pow(10,9); // gigagram 10^9

	double distance2D(double x1, double y1, double x2, double y2);

	// Calculate object 1s speed of acceleration towards object 2
	double calculateGravityAccel(double x1, double y1, double x2, double y2, double mass2);
}