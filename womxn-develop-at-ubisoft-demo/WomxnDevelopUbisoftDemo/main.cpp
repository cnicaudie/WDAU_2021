#include <stdafx.h>

#include <Game/GameManager.h>

int main()
{
    GameManager* game = GameManager::GetInstance();
    game->RunGameLoop();

    return EXIT_SUCCESS;
}