#include "game.h"

int main() {
    Game game(800, 600);
    game.selectLevel("assets/level1.txt");
    game.run();
    return 0;
}
