#include "SnakePlusPlus.h"

int WINAPI WinMain (HINSTANCE instance, HINSTANCE, LPSTR, int) {

    const int positionX = 50, positionY = 50;
    const int cellsX = 60, cellsY = 30;
    const int size = 30;
    const bool fullscreen = true;

    Game snakeGame(instance, fullscreen, size, cellsX, cellsY, positionX, positionY);

    return snakeGame.run();

}
