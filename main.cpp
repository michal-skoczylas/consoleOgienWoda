#include "game.h"

int main() {
    Game game(800, 600);
    game.selectLevel("assets/level3.txt");
    game.run();
    return 0;
}
