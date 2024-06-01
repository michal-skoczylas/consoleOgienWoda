
#include "level.h"
#include <fstream>
#include <iostream>
#include <map>
Level::Level() {
    sf::RectangleShape platform(sf::Vector2f(200,20));
    platform.setFillColor(sf::Color::Green);
    platform.setPosition(100,500);
    platforms.emplace_back(platform);
}
void Level::draw(sf::RenderWindow& window){
    for(const auto& platform : platforms){
        window.draw(platform);
    }
    // Draw player start position (yellow tile)
    sf::RectangleShape startTile(tileSize); // Adjust size as needed
    startTile.setFillColor(sf::Color::Yellow); // Adjust color as needed
    startTile.setPosition(playerStart);
    window.draw(startTile);

    // Draw goal position (red tile)
    sf::RectangleShape goalTile(tileSize); // Adjust size as needed
    goalTile.setFillColor(sf::Color::Red); // Adjust color as needed
    goalTile.setPosition(goal);
    window.draw(goalTile);
    for(const auto&tiles :walls){
        window.draw(tiles);
    }
}


void Level::checkCollisions(Player& player) {
    sf::FloatRect playerBounds = player.getBounds();
    for (const auto& platform : platforms) {
        if (platform.getGlobalBounds().intersects(playerBounds)) {
            player.handleCollision(platform);
        }
    }
}

void Level::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open level file");
    }

    // Wczytaj rozmiar mapy i rozmiar kafelka
    int mapWidth, mapHeight;
    int tileSizeX, tileSizeY;
    file >> mapWidth >> mapHeight >> tileSizeX >> tileSizeY;

    // Utwórz mapę asocjacyjną dla kafli
    std::map<std::pair<int, int>, char> tileMap;

    // Wczytaj kafelki z pliku tekstowego
    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        for (int x = 0; x < line.size(); ++x) {
            char tileType = line[x];
            tileMap[{x, y}] = tileType;

            switch (tileType) {
            case '#': {
                // Dodaj ścianę
                sf::RectangleShape wall(sf::Vector2f(tileSizeX, tileSizeY));
                wall.setFillColor(sf::Color::Blue);
                wall.setPosition(x * tileSizeX, y * tileSizeY);
                walls.push_back(wall);
                break;
            }
            case '@': {
                sf::RectangleShape coin(sf::Vector2f(tileSizeX,tileSizeY));
                coin.setFillColor(sf::Color::Yellow);
                coin.setPosition(x*tileSizeX,y*tileSizeY);
                walls.emplace_back(coin);
                break;
            }
            case '$': {
                // Ustaw pozycję celu
                goal = sf::Vector2f(x * tileSizeX, y * tileSizeY);
                break;
            }
            default:
                // Obsłuż inne typy kafli
                break;
            }
        }
        y++;
    }

    // Zamknij plik
    file.close();
}
