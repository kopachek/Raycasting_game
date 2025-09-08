#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>

static class TextureManager {
private:
	static std::unordered_map<int, std::unique_ptr<sf::Texture>> textures;

public:
	static sf::Texture* getTexture(int id);
	static bool loadTexture(int id, const std::string& path);
};