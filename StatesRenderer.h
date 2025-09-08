#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "TextureManager.h"

class StatesRenderer
{
public:
    void renderMainMenu(sf::RenderWindow& window);
    void renderWinScreen(sf::RenderWindow& window);
};

