#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Map.h"
#include "Utility.h"

class Player
{
public:
	sf::Vector2f coordinates;
	float rotationAngle;
	int moveSpeed;
	int boundingSquareRadius;
	int collisionOffset;
	int deltaHeight;


	Player(sf::Vector2f coordinates, float rotationAngle, float moveSpeed, int boundingSquareRadius, int collisionOffset, int deltaHeight);
	//sf::Keyboard::Key inputHandler();
	void move(Map* map, float directionAngleOffset);
	void rotateHorizontally(float angle);
	void rotateVertically(float delta);
	int frontWallTrigger(Map* map, float length); //Returns the Id of the wall hit in front of the player on certain length 
	void render(sf::RenderWindow& window, Map* map);
};

