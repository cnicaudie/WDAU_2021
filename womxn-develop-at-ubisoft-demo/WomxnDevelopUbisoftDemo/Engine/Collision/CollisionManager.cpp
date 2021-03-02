#include <stdafx.h>
#include "CollisionManager.h"
#include <Game/Map/CollideableTile.h>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckCollision(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const
{
    bool isColliding = false;
    
    sf::FloatRect playerCollider = player.GetBoundingBox();
    playerCollider.left += nextPosition.x;
    playerCollider.top += nextPosition.y;

    sf::FloatRect otherCollider;

    for (auto& tile : map.GetTileMap())
    {
        if (std::shared_ptr<CollideableTile> t = std::dynamic_pointer_cast<CollideableTile>(tile)) 
        {
            otherCollider = t->GetBoundingBox();
        
            if (otherCollider.intersects(playerCollider))
            {
                isColliding = true;

                // Check the direction of the collision 

                // Bottom collision
                if (playerCollider.top < otherCollider.top && playerCollider.top + playerCollider.height < otherCollider.top + otherCollider.height)
                {
                    player.SetGroundLevel(true);
                    player.ResetVelocityY();
                    player.SetPositionY(otherCollider.top - (playerCollider.height / 2));
                    break;
                }

                // Top collision
                else if (playerCollider.top > otherCollider.top && playerCollider.top + playerCollider.height > otherCollider.top + otherCollider.height)
                {
                    player.ResetVelocityY();
                    player.SetPositionY(otherCollider.top + otherCollider.height + (playerCollider.height / 2) + 0.5f);
                    break;
                }

                // Right collision
                else if (playerCollider.left < otherCollider.left && playerCollider.left + playerCollider.width < otherCollider.left + otherCollider.width)
                {
                    player.ResetVelocityX();
                    player.SetPositionY(otherCollider.left - (playerCollider.width / 2) - 0.5f);
                    break;
                }

                // Left collision
                else if (playerCollider.left > otherCollider.left && playerCollider.left + playerCollider.width > otherCollider.left + otherCollider.width)
                {
                    player.ResetVelocityX();
                    player.SetPositionY(otherCollider.left + otherCollider.width + (playerCollider.width / 2) + 0.5f);
                    break;
                }
            }
        }
    }

    return isColliding;
}