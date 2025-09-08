#include "Ray.h"

void Ray::cast(Map* map) {
	bool isFacingRight = false;
	bool isFacingDown = false;
	
	if (rotationAngle > 3 * M_PI_2 || rotationAngle < M_PI_2)
		isFacingRight = true;

	if (rotationAngle > 0 && rotationAngle < M_PI)
		isFacingDown = true;

	int wallNumH = 0;
	int wallNumV = 0;

	//Horizontal walls check

	isHorizontalWallHit = false;
	sf::Vector2f horizontalWallHit(coordinates.x, coordinates.y);
	sf::Vector2f firstHorizontalIntersection;
	sf::Vector2f nextHorizontalIntersection(coordinates.x, coordinates.y);

	if (isFacingDown)
		firstHorizontalIntersection.y = int(coordinates.y / settings.tileSize) * settings.tileSize + settings.tileSize;
	else
		firstHorizontalIntersection.y = int(coordinates.y / settings.tileSize) * settings.tileSize - 1;

	firstHorizontalIntersection.x = coordinates.x + (firstHorizontalIntersection.y - coordinates.y) / std::tan(rotationAngle);
	nextHorizontalIntersection.x = firstHorizontalIntersection.x;
	nextHorizontalIntersection.y = firstHorizontalIntersection.y;

	float deltaX;
	float deltaY;

	if (isFacingDown)
		deltaY = settings.tileSize;
	else
		deltaY = -settings.tileSize;

	deltaX = deltaY / std::tan(rotationAngle);
	
	sf::Vector2i nextHorizontalTile = map->toTileCoordinates(nextHorizontalIntersection);
	while (nextHorizontalTile.x >= 0
		&& nextHorizontalTile.x < map->columns
		&& nextHorizontalTile.y >= 0
		&& nextHorizontalTile.y < map->rows) 
	{
		if (map->mapArray[nextHorizontalTile.y][nextHorizontalTile.x] > 0) {
			isHorizontalWallHit = true;
			horizontalWallHit.x = nextHorizontalIntersection.x;
			if (isFacingDown)
				horizontalWallHit.y = nextHorizontalIntersection.y;
			else 
				horizontalWallHit.y = nextHorizontalIntersection.y;
			wallNumH = map->mapArray[nextHorizontalTile.y][nextHorizontalTile.x];
			break;
		}
		else {
			nextHorizontalIntersection.x += deltaX;
			nextHorizontalIntersection.y += deltaY;
			nextHorizontalTile = map->toTileCoordinates(nextHorizontalIntersection);
		}
	}

	if (!isHorizontalWallHit) {
		horizontalWallHit.x = nextHorizontalIntersection.x;
		if (isFacingDown)
			horizontalWallHit.y = nextHorizontalIntersection.y;
		else
			horizontalWallHit.y = nextHorizontalIntersection.y;
	}


	//Vertical walls check

	isVerticalWallHit = false;
	sf::Vector2f verticalWallHit(coordinates.x, coordinates.y);
	sf::Vector2f firstVerticalIntersection(coordinates.x, coordinates.y);
	sf::Vector2f nextVerticalIntersection(coordinates.x, coordinates.y);

	if (isFacingRight)
		firstVerticalIntersection.x = int(coordinates.x / settings.tileSize) * settings.tileSize + settings.tileSize;
	else
		firstVerticalIntersection.x = int(coordinates.x / settings.tileSize) * settings.tileSize - 1;

	firstVerticalIntersection.y = coordinates.y + (firstVerticalIntersection.x - coordinates.x) * std::tan(rotationAngle);
	nextVerticalIntersection.x = firstVerticalIntersection.x;
	nextVerticalIntersection.y = firstVerticalIntersection.y;

	if (isFacingRight)
		deltaX = settings.tileSize;
	else
		deltaX = -settings.tileSize;

	deltaY = deltaX * std::tan(rotationAngle);
	
	sf::Vector2i nextVerticalTile = map->toTileCoordinates(nextVerticalIntersection);
	while (nextVerticalTile.x >= 0
		&& nextVerticalTile.x < map->columns
		&& nextVerticalTile.y >= 0
		&& nextVerticalTile.y < map->rows ) 
	{
		if (map->mapArray[nextVerticalTile.y][nextVerticalTile.x] > 0) {
			isVerticalWallHit = true;
			if(isFacingRight)
				verticalWallHit.x = nextVerticalIntersection.x;
			else 
				verticalWallHit.x = nextVerticalIntersection.x;
			verticalWallHit.y = nextVerticalIntersection.y;
			wallNumV = map->mapArray[nextVerticalTile.y][nextVerticalTile.x];
			break;
		}
		else {
			nextVerticalIntersection.x += deltaX;
			nextVerticalIntersection.y += deltaY;
			nextVerticalTile = map->toTileCoordinates(nextVerticalIntersection);
		}
	}

	if (!isVerticalWallHit) {
		verticalWallHit.y = nextVerticalIntersection.y;
		if (isFacingRight)
			verticalWallHit.x = nextVerticalIntersection.x;
		else
			verticalWallHit.x = nextVerticalIntersection.x;
	}

	float horizontalLength = std::pow((horizontalWallHit.x - coordinates.x), 2) + std::pow((horizontalWallHit.y - coordinates.y), 2);
	float verticalLength = std::pow((verticalWallHit.x - coordinates.x), 2) + std::pow((verticalWallHit.y - coordinates.y), 2);

	if (horizontalLength > verticalLength) {
		wallHitCoordinates = verticalWallHit;
		length = std::sqrtf(verticalLength);
		isVerticalWallHit = true;
		isHorizontalWallHit = false;
		wallNum = wallNumV;
	}
	else {
		wallHitCoordinates = horizontalWallHit;
		length = std::sqrtf(horizontalLength);
		isVerticalWallHit = false;
		isHorizontalWallHit = true;
		wallNum = wallNumH;
	}
}

void Ray::render(sf::RenderWindow& window, Map* map) {
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	float size = settings.projectionWindowSize().y / (3.f * map->columns);
	line[0].position = sf::Vector2f(coordinates.x * size / settings.tileSize, coordinates.y * size / settings.tileSize);
	line[0].color = sf::Color::Red;
	line[1].position = sf::Vector2f(wallHitCoordinates.x * size / settings.tileSize, wallHitCoordinates.y * size / settings.tileSize);
	line[1].color = sf::Color::Red;
	
	window.draw(line);
}

Ray::Ray(sf::Vector2f coordinates, double rotationAngle) {
	this->coordinates = coordinates;
	this->rotationAngle = utility.rNormalizeAngle(rotationAngle);
	wallHitCoordinates = coordinates;
}