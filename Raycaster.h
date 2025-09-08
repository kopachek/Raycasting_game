#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Ray.h"
#include "Map.h"
#include "Player.h"
#include "Settings.h"

class Raycaster
{
public:
	std::vector<Ray> rays;

	void generateRays(Player* player, Map* map);
	void renderRays(sf::RenderWindow& window, Map* map);
};

