#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Settings.h"

class Map
{
public:
	std::vector<std::vector<int>> mapArray
	{ 
		{1,1,0,1,1,1,1,0,1,1},
		{1,0,0,1,0,0,1,0,0,1},
		{1,0,0,1,0,0,1,1,0,1},
		{1,0,0,1,0,1,1,0,0,1},
		{1,0,0,1,0,0,1,0,0,1},
		{1,0,0,1,0,0,1,0,0,1},
		{0,0,0,1,0,0,1,0,0,1},
		{1,0,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,0,1,1,1},
	};
	int rows;
	int columns;

	void render(sf::RenderWindow&);
	bool loadFromFile(const std::string& path);
	sf::Vector2i toTileCoordinates(sf::Vector2f windowCoordinates);
};

