#pragma once
#include <SFML/Graphics.hpp>

using uint = unsigned int;

static struct Settings {
    float fov = 60;
    float fovRad = fov * M_PI / 180;
    int tileSize = 128;
    double resolution = 5;
    int maxFramerate = 60;

    int viewingDist = 8; //In tiles
    float horizontalSensitivity = 0.03;
    float verticalSensitivity = 0.3;

    sf::Vector2u projectionWindowSize() {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        return sf::Vector2u(desktop.size);
    }

    float planeDist() {
        return projectionWindowSize().x / (2 * std::tan(fovRad / 2));
    }

    int numberOfRays() {
        return projectionWindowSize().x / resolution;
    }

    
};

static Settings settings;