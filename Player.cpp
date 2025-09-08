#include "Player.h"
#include <cmath>
#include <iostream>

void Player::move(Map* map, float directionAngleOffset) {
	
	float tempX = coordinates.x + std::cos(rotationAngle + directionAngleOffset) * moveSpeed;
	float tempY = coordinates.y + std::sin(rotationAngle + directionAngleOffset) * moveSpeed;

	sf::Vector2f cornersOld[] = {
		sf::Vector2f(coordinates.x - boundingSquareRadius - collisionOffset, coordinates.y - boundingSquareRadius - collisionOffset),
		sf::Vector2f(coordinates.x + boundingSquareRadius + collisionOffset, coordinates.y - boundingSquareRadius - collisionOffset),
		sf::Vector2f(coordinates.x + boundingSquareRadius + collisionOffset, coordinates.y + boundingSquareRadius + collisionOffset),
		sf::Vector2f(coordinates.x - boundingSquareRadius - collisionOffset, coordinates.y + boundingSquareRadius + collisionOffset)
	};

	sf::Vector2f cornersNew[] = {
		sf::Vector2f(tempX - boundingSquareRadius - collisionOffset, tempY - boundingSquareRadius - collisionOffset),
		sf::Vector2f(tempX + boundingSquareRadius + collisionOffset, tempY - boundingSquareRadius - collisionOffset),
		sf::Vector2f(tempX + boundingSquareRadius + collisionOffset, tempY + boundingSquareRadius + collisionOffset),
		sf::Vector2f(tempX - boundingSquareRadius - collisionOffset, tempY + boundingSquareRadius + collisionOffset)
	};
	bool canMoveX = true;
	bool canMoveY = true;
	for (int i = 0; i < 4; i++) {
		sf::Vector2i tileCoordsOld = map->toTileCoordinates(cornersOld[i]);
		sf::Vector2i tileCoordsNew = map->toTileCoordinates(cornersNew[i]);
		if (tileCoordsNew.x < 0 || tileCoordsOld.y >= map->rows || tileCoordsNew.x >= map->columns || map->mapArray[tileCoordsOld.y][tileCoordsNew.x] > 0)
			canMoveX = false;
		if (tileCoordsNew.y < 0 || tileCoordsNew.y >= map->rows || tileCoordsOld.x >= map->columns || map->mapArray[tileCoordsNew.y][tileCoordsOld.x] > 0)
			canMoveY = false;
	}
	if(canMoveX)
		coordinates.x = tempX;
	if(canMoveY)
		coordinates.y = tempY;
}

void Player::rotateHorizontally(float angle) {
	float newAngle = rotationAngle + settings.horizontalSensitivity * angle * M_PI / 180;
	this->rotationAngle = utility.rNormalizeAngle(newAngle);
}

void Player::rotateVertically(float delta) {
	float newHeight = deltaHeight + delta * settings.verticalSensitivity;
	if (std::abs(newHeight) < settings.projectionWindowSize().y / 2)
		this->deltaHeight = newHeight;
}

void Player::render(sf::RenderWindow& window, Map* map) {
	sf::CircleShape player(2);
	player.setFillColor(sf::Color::Red);
	float size = settings.projectionWindowSize().y / (3.f * map->columns);
	player.setPosition(sf::Vector2f((coordinates.x - boundingSquareRadius) * size/ settings.tileSize, (coordinates.y - boundingSquareRadius) * size / settings.tileSize));
	window.draw(player);
}

int Player::frontWallTrigger(Map* map, float length) {
	sf::Vector2f hitCoords = sf::Vector2f(coordinates.x + cos(rotationAngle) * length, coordinates.y + std::sin(rotationAngle) * length);
	sf::Vector2i tileHit = map->toTileCoordinates(hitCoords);
	if (tileHit.x < 0
		|| tileHit.x > map->columns
		|| tileHit.y < 0
		|| tileHit.y > map->rows)
		return 0;
	else
		return map->mapArray[tileHit.y][tileHit.x];
}

Player::Player(sf::Vector2f coordinates, float rotationAngle, float moveSpeed, int boundingSquareRadius, int collisionOffset, int deltaHeight) {
	this->coordinates = coordinates;
	this->rotationAngle = utility.rNormalizeAngle(rotationAngle);
	this->moveSpeed = moveSpeed;
	this->boundingSquareRadius = boundingSquareRadius;
	this->collisionOffset = collisionOffset;
	if (std::abs(deltaHeight) < settings.projectionWindowSize().y / 2)
		this->deltaHeight = deltaHeight;
	else
		this->deltaHeight = 0;
}