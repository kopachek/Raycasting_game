#include "TextureManager.h"

std::unordered_map<int, std::unique_ptr<sf::Texture>> TextureManager::textures;

sf::Texture* TextureManager::getTexture(int id) {
	auto it = textures.find(id);
	if (it != textures.end())
		return it->second.get();
	auto defaultIt = textures.find(0);
	return defaultIt->second.get();
}

bool TextureManager::loadTexture(int id, const std::string& path) {
	std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
	if (!texture->loadFromFile(path))
		return false;
	else
		textures[id] = std::move(texture);
	return true;
}