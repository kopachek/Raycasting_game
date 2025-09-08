#include "Map.h"

void Map::render(sf::RenderWindow& window) {
	float size = settings.projectionWindowSize().y / (3.f * columns); 

	sf::RectangleShape whiteRect(sf::Vector2f(size, size));
	whiteRect.setFillColor(sf::Color(0xe6, 0xe6, 0xe6));

	sf::RectangleShape anotherRect(sf::Vector2f(size, size));
	anotherRect.setFillColor(sf::Color(0x38, 0x38, 0x38));

	for (int i = 0; i < mapArray.size(); i++) {
		for (int j = 0; j < mapArray[i].size(); j++) {
			sf::Vector2f pos(j * size, i * size);
			if (mapArray[i][j] == 0) {
				whiteRect.setPosition(pos);
				window.draw(whiteRect);
			}
			if (mapArray[i][j] > 0) {
				anotherRect.setPosition(pos);
				window.draw(anotherRect);
			}
		}
	}
}

sf::Vector2i Map::toTileCoordinates(sf::Vector2f windowCoords) {
	sf::Vector2i tileCoords = sf::Vector2i(windowCoords.x / settings.tileSize, windowCoords.y / settings.tileSize);
	if (windowCoords.x < 0)
		tileCoords.x -= 1;
	if (windowCoords.y < 0)
		tileCoords.y -= 1;
	return tileCoords;
}

bool Map::loadFromFile(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open())
		return false;

	mapArray.clear();
	std::string line;

	while (std::getline(file, line)) {
		std::vector<int> row;
		for (char ch : line) {
			if (isdigit(ch)) {
				row.push_back(ch - '0');
			}
		}
		mapArray.push_back(row);
	}

	if (mapArray.empty())
		throw std::runtime_error("Invalid map format");
	columns = mapArray[0].size();
	rows = mapArray.size();

	return true;
}