#include <iostream>
#include<SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Settings.h"
#include "Map.h"
#include "Player.h"
#include "Ray.h"
#include "Raycaster.h"
#include "SceneRenderer.h"
#include "TextureManager.h"
#include "LoadingManager.h"
#include "StatesRenderer.h"

enum States
{
    MainMenu,
    Running,
    WinScreen
};
int main()
{
    sf::RenderWindow window(sf::VideoMode::VideoMode(settings.projectionWindowSize()), "RayMaze", sf::Style::None);
    Map *map = new Map();
    Player* player = new Player(
        sf::Vector2f(settings.tileSize * 5, settings.tileSize * 5),
        3 * M_PI_2,
        3.f / settings.maxFramerate * settings.tileSize, 
        2,
        2,
        0
    );
    Raycaster* raycaster = new Raycaster();
    SceneRenderer* sceneRenderer = new SceneRenderer();
    StatesRenderer* statesRenderer = new StatesRenderer();
    States state = MainMenu;

    sf::Music music;
    if (!music.openFromFile("Music\\bgm.ogg")) {
        std::cerr << "Fatal error: Couldn't load music" << std::endl;
        return 1;
    }
    bool musicOn = true;
    float musicVolume = 50.f;
    music.setVolume(musicVolume);
    music.setLooping(true);
    music.play();

    int level = 1;
    int maxLevel = 3;
    bool ended = false;
    bool loadLevel = true;

    try {
        if (!LoadingManager::loadTextures())
            throw std::runtime_error("Failed to load textures");
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    window.setFramerateLimit(settings.maxFramerate);
    sf::Vector2i mousePos(settings.projectionWindowSize().x / 2, settings.projectionWindowSize().y / 2);
    sf::Mouse::setPosition(mousePos, window);
    
    while (window.isOpen())
    {
        if (ended)
            break;
        if (level > maxLevel)
            state = States::WinScreen;
        if (loadLevel) {
            if (level <= maxLevel) {
                std::string path = "Maps\\map" + std::to_string(level) + ".txt";
                try {
                    if (!LoadingManager::loadSurrounding(player, map, path))
                        throw std::runtime_error("Failed to load surrounding");
                }
                catch (const std::exception& e) {
                    std::cerr << "Fatal error: " << e.what() << std::endl;
                    return 1;
                }
            }
            loadLevel = false;
        }

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (state == States::MainMenu)
                    state = States::Running;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    if (state == States::WinScreen)
                        ended = true;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::M) {
                    if (musicOn) {
                        music.setVolume(0);
                        musicOn = false;
                    }
                    else {
                        music.setVolume(musicVolume);
                        musicOn = true;
                    }
                }
            }

            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) && state == States::Running) 
                {
                    int deltaX = mouseMoved->position.x - mousePos.x;
                    int deltaY = mouseMoved->position.y - mousePos.y;
                    player->rotateVertically(deltaY);
                    player->rotateHorizontally(deltaX);
                    sf::Mouse::setPosition(mousePos, window);
                }
            }
        }
        switch (state) {
            case States::MainMenu: {
                window.clear();
                statesRenderer->renderMainMenu(window);
                window.display();
                break;
            }

            case States::Running: {
                int frontWall = player->frontWallTrigger(map, settings.tileSize / 3);
                if (frontWall == 9) {
                    level++;
                    loadLevel = true;
                    break;
                }
                sf::Vector2f directionVector(0, 0);
                int moveSpeedSave = player->moveSpeed;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) directionVector.x += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) directionVector.y -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) directionVector.x -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) directionVector.y += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) player->moveSpeed *= 2;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt))
                    window.setMouseCursorVisible(true);
                else
                    window.setMouseCursorVisible(false);

                if (directionVector.x != 0 || directionVector.y != 0) {
                    double directionAngle = std::atan2(directionVector.y, directionVector.x);
                    player->move(map, directionAngle);
                }
                player->moveSpeed = moveSpeedSave;

                raycaster->generateRays(player, map);

                window.clear();
                sceneRenderer->renderFloor(player, window);
                sceneRenderer->renderCeiling(player, window);
                sceneRenderer->renderWalls(raycaster, player, map, window);
                sceneRenderer->renderCompass(player, window);

                window.display();
                break;
            }

            case States::WinScreen: {
                window.clear();
                statesRenderer->renderWinScreen(window);
                window.display();
            }

            default: {
                break;
            }
        }
    }
    delete map;
    delete player;
    delete raycaster;
    delete sceneRenderer;
    return 0;
}