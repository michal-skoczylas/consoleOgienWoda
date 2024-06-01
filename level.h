#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include "player.h"
#include <vector>
class Level
{
public:
    Level();
    void draw(sf::RenderWindow& window);
    void checkCollisions(Player& player);
    void loadFromFile(const std::string& filename);

    private:
    std::vector<sf::RectangleShape> platforms;
    std::vector<sf::RectangleShape> walls;
    sf::Vector2f playerStart;
    sf::Vector2f goal;
    sf::Vector2f tileSize=sf::Vector2f(18,13.5);
};

#endif // LEVEL_H
