#include <stdafx.h>
#include "CollisionManager.h"
#include <Game/Map/CollideableTile.h>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckCollisionX(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const
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

                // Left collision
                if (playerCollider.left + playerCollider.width > otherCollider.left
                    && playerCollider.left < otherCollider.left)
                {
                    player.ResetVelocityX();
                    //player.SetPositionX(otherCollider.left + otherCollider.width + (playerCollider.width / 2));
                    std::cout << "Left Collision" << std::endl;
                }
                // Right collision
                else if (playerCollider.left < otherCollider.left + otherCollider.width
                    && playerCollider.left + playerCollider.width > otherCollider.left + otherCollider.width)
                {
                    player.ResetVelocityX();
                    //player.SetPositionX(otherCollider.left - (playerCollider.width / 2));
                    std::cout << "Right Collision" << std::endl;
                }


            }
        }
    }

    return isColliding;
}

const bool CollisionManager::CheckCollisionY(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const
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
                if (playerCollider.top + playerCollider.height > otherCollider.top 
                    && playerCollider.top < otherCollider.top)
                {
                    player.SetGroundLevel(true);
                    player.ResetVelocityY();
                    player.SetPositionY(otherCollider.top - (playerCollider.height / 2));
                }               
                
                // Top collision
                else if (playerCollider.top < otherCollider.top + otherCollider.height
                    && playerCollider.top + playerCollider.height > otherCollider.top + otherCollider.height)
                {
                    player.ResetVelocityY();
                    player.SetPositionY(otherCollider.top + otherCollider.height + (playerCollider.height / 2));
                    std::cout << "Top Collision" << std::endl;
                }

            }
        }
    }

    return isColliding;
}