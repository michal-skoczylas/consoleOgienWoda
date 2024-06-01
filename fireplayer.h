#ifndef FIREPLAYER_H
#define FIREPLAYER_H
#include "player.h"
class FirePlayer: public Player
{
public:
    FirePlayer(sf::Vector2f position,sf::Color color);
    void handleInput() override;
};

#endif // FIREPLAYER_H
