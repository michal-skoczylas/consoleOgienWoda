#ifndef TILES_H
#define TILES_H
#include "SFML/Graphics.hpp"

class Tiles:public sf::Sprite
{
public:
    Tiles();
    sf::Sprite sprite;

};


class WaterTile:public Tiles{

};
class LavaTile:public Tiles{

};

class AcidTile:public Tiles{

};
class SlipperyTile:public Tiles{

};
class Mud:public Tiles{

};

#endif // TILES_H
