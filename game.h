#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"
#include "fireplayer.h"
#include "waterplayer.h"
#include <vector>

class Game
{
public:
    Game(int windowWidth,int windowHeight);
    void run();
private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    FirePlayer player1;
    WaterPlayer player2;
    sf::Texture player1Texture;
    sf::Texture player2Texture;
    Level level;
    std::vector<Player*> players={&player1,&player2};
};

#endif // GAME_H
