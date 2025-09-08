#include "LoadingManager.h"

bool LoadingManager::loadSurrounding(Player* player, Map* map, const std::string& mapPath) {
	if(!map->loadFromFile(mapPath))
		return false;
	
	sf::Vector2f playerCoords;
	bool coordsSet = false;
	for (int i = 0; i < map->rows; i++) {
		for (int j = 0; j < map->columns; j++) {
			if (map->mapArray[i][j] == 8) {
				playerCoords = sf::Vector2f((j + 0.5) * settings.tileSize, (i + 0.5) * settings.tileSize);
				coordsSet = true;
				map->mapArray[i][j] = 0;
			}
		}
	}
	if (!coordsSet)
		return false;

	player->coordinates = playerCoords;
	return true;
}

bool LoadingManager::loadTextures() {
	if(!TextureManager::loadTexture(0, "Textures\\default.png"))
		return false;
	if (!TextureManager::loadTexture(1, "Textures\\TECH_1E.png"))
		return false;
	if (!TextureManager::loadTexture(2, "Textures\\TECH_2F.png"))
		return false;
	if (!TextureManager::loadTexture(3, "Textures\\TECH_3B.png"))
		return false;
	if (!TextureManager::loadTexture(4, "Textures\\TECH_4F.png"))
		return false;
	if (!TextureManager::loadTexture(5, "Textures\\CONSOLE_1B.png"))
		return false;
	if (!TextureManager::loadTexture(6, "Textures\\PIPES_1A.png"))
		return false;
	if (!TextureManager::loadTexture(7, "Textures\\DOOR_1E.png"))
		return false;
	if (!TextureManager::loadTexture(9, "Textures\\DOOR_1E.png"))
		return false;
	if (!TextureManager::loadTexture(10, "Textures\\menu.jpg"))
		return false;
	return true;
}