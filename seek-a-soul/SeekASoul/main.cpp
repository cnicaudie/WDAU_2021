#include <stdafx.h>
#include <Game/GameManager.h>

int main()
{
    SeekASoul::Gameplay::GameManager* game = SeekASoul::Gameplay::GameManager::GetInstance();
    game->RunGameLoop();

    return EXIT_SUCCESS;
}