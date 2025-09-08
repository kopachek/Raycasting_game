#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
struct Utility {
	void normalizeAngle(double& angle) {
		if (angle >= 2 * M_PI)
			angle = std::fmod(angle, 2 * M_PI);
		else if (angle < 0)
			angle = std::fmod(angle, 2 * M_PI) + 2 * M_PI;
	}
	double rNormalizeAngle(double angle) {
		if (angle >= 2 * M_PI)
			return angle = std::fmod(angle, 2 * M_PI);
		else if (angle < 0)
			return angle = std::fmod(angle, 2 * M_PI) + 2 * M_PI;
		else return angle;
	}
	double toRadians(double angle) {
		return angle * M_PI / 180;
	}
};

static Utility utility;