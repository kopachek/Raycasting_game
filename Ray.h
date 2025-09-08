#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Map.h"
#include "Player.h"
#include "Utility.h"
#include "Settings.h"

class Ray
{
public:
	sf::Vector2f coordinates;
	double rotationAngle;
	sf::Vector2f wallHitCoordinates;
	bool isHorizontalWallHit = false;
	bool isVerticalWallHit = false;
	double length;
	int wallNum;

	void cast(Map* map);
	void render(sf::RenderWindow& window, Map* map);
	Ray(sf::Vector2f coordinates, double rotationAngle);
};

