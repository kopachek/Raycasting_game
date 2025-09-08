#pragma once
#include "Player.h"
#include "Map.h"
#include "Settings.h"
#include "TextureManager.h"
#include <string>

static class LoadingManager
{
public:
	static bool loadSurrounding(Player* player, Map* map, const std::string& mapPath);
	static bool loadTextures();
};

