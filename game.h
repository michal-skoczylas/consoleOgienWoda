#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "player.h"
#include "level.h"
#include "fireplayer.h"
#include "waterplayer.h"

class Game
{
public:
    Game();
    void run();
private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow window;
    FirePlayer player1;
    WaterPlayer player2;
    Level level;

};

#endif // GAME_H
