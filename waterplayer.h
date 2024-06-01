#ifndef WATERPLAYER_H
#define WATERPLAYER_H
#include "player.h"
class WaterPlayer:public Player
{
public:
    WaterPlayer(sf::Vector2f position,sf::Color color);
    void handleInput() override;
};

#endif // WATERPLAYER_H
