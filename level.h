#ifndef LEVEL_H
#define LEVEL_H
#include <SFML/Graphics.hpp>
#include "player.h"
#include "tiles.h"
#include <vector>
#include <memory>
class Level
{
public:
    Level();
    void draw(sf::RenderWindow& window);
    void checkCollisions(Player& player);
    void checkCollisions(const std::vector<Player*>& players);
    void loadFromFile( std::string& filename);
    void loadTextures(std::string texture_loader_filepath);
    bool chekGoalReached(Player& player);
    bool checkGemReached(Player& player);
    void checkEndGame(std::vector<Player*> players);
    void setLevelFinished();
    bool getLevelFinished();
    void endGame(); 
    void saveBestTime();
    std::vector<std::shared_ptr<Tiles>> ptr_sprites;
    void setFinalTime(std::string time);
        std::string getLevelPath();
        std::string constructTimeFilePath(const std::string& levelPath);
    std::string arglevelPath;
    std::string  getFinalTime();
    std::string getLevelPath() const { return levelPath; }
    sf::Vector2f getStartingPosition();
sf::Vector2f getFireStartingPosition();
sf::Vector2f getWaterStartingPosition();
bool getGemCollected();
bool isGem(const sf::Sprite& sprite);
bool isReceiver(const sf::Sprite& sprite);
    private:
    sf::Sprite goalTile;
    sf::Sprite gemTile;
    sf::Sprite buttonTile;
    sf::Sprite receiverTile;
    std::vector<sf::RectangleShape> platforms;
    std::vector<sf::RectangleShape> walls;
    std::vector<sf::Sprite> sprites;
    std::vector<sf::Sprite> lavaSprites;
    std::vector <sf::Sprite> waterSprites;
    std::vector<sf::Sprite> acidSprites;
    std::vector<sf::Sprite> slippery_wallsSprites;
    std::vector<sf::Sprite> gemSprites;
    std::vector<sf::Sprite> mudSprites;
    std::vector<sf::Sprite> buttonSprites;
    std::vector<sf::Sprite> receiverSprites;
    std::vector<sf::Sprite> spritesToRemove;
    sf::Vector2f playerStart;
    sf::Vector2f goal;
    sf::Vector2f gem;
    sf::Vector2f button;
    sf::Vector2f tileSize=sf::Vector2f(18,13.5);
    std::vector<sf::Texture> textures;
    bool levelFinished=false;
    bool gemCollected=false;
    bool isButtonPressed=false;
    bool isLoaded = false;
    //Pozycje startowe graczy dla obu takie same
    sf::Vector2f startingPosition;
    sf::Sprite startingTile;
    //Pozycje startowe dla graczy inne
    sf::Vector2f fireStartingPosition;
    sf::Vector2f waterStartingPosition;
    sf::Sprite fireStartingTile;
    sf::Sprite waterStartingTile;
    std::string levelPath;
    std::string getFinalTime() const;
    std::string finalTime;
};

#endif // LEVEL_H
