#include "game.h"

int main() {
    Game game(800, 600);
    game.selectLevel("D:/klonygithub/consoleOgienWoda/assets/level3.txt");
    game.run();
    return 0;
}
