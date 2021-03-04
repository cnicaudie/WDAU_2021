#include <stdafx.h>

int main()
{
    GameManager* game = GameManager::GetInstance();
    game->RunGameLoop();

    return EXIT_SUCCESS;
}