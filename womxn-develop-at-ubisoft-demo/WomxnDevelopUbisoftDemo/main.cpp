#include <stdafx.h>

#include <Game/GameManager.h>

int main()
{
    GameManager game{};
    game.RunGameLoop();

    return EXIT_SUCCESS;
}