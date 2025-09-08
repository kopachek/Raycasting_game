#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Raycaster.h"
#include "Map.h"
#include "Utility.h"
#include "TextureManager.h"
#include <string>
#include <vector>
#include <map>

class SceneRenderer
{
public:

	void renderWalls(Raycaster* raycaster, Player* player, Map* map, sf::RenderWindow& window);
	void renderFloor(Player* player, sf::RenderWindow& window);
	void renderCeiling(Player* player, sf::RenderWindow& window);
	void renderCompass(Player* player, sf::RenderWindow& window);
};

