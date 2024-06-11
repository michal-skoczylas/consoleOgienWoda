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
    void loadFromFile( std::string& filename);
    void loadTextures(std::string texture_loader_filepath);
    bool chekGoalReached(Player& player);
    void checkEndGame(std::vector<Player*> players);
    void setLevelFinished();
    bool getLevelFinished();
    void endGame(); 
    sf::Vector2f getStartingPosition();
sf::Vector2f getFireStartingPosition();
sf::Vector2f getWaterStartingPosition();
    private:
    sf::Sprite goalTile;
    std::vector<sf::RectangleShape> platforms;
    std::vector<sf::RectangleShape> walls;
    std::vector<sf::Sprite> sprites;
    sf::Vector2f playerStart;
    sf::Vector2f goal;
    sf::Vector2f tileSize=sf::Vector2f(18,13.5);
    std::vector<sf::Texture> textures;
    bool levelFinished=false;
    //Pozycje startowe graczy dla obu takie same
    sf::Vector2f startingPosition;
    sf::Sprite startingTile;
    //Pozycje startowe dla graczy inne
    sf::Vector2f fireStartingPosition;
    sf::Vector2f waterStartingPosition;
    sf::Sprite fireStartingTile;
    sf::Sprite waterStartingTile;
};

#endif // LEVEL_H
