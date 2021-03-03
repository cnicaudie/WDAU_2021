#include <stdafx.h>

#include "CollisionManager.h"

#include <Game/Map/CollideableTile.h>


#include <utility>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckPlayerCollisionX(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const
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
                    player.SetPositionX(otherCollider.left - (playerCollider.width / 2));
                }
                
                // Right collision
                else if (playerCollider.left < otherCollider.left + otherCollider.width
                    && playerCollider.left + playerCollider.width > otherCollider.left + otherCollider.width)
                {
                    player.ResetVelocityX();
                    player.SetPositionX(otherCollider.left + otherCollider.width + (playerCollider.width / 2));
                }
            }
        }
    }

    return isColliding;
}

const bool CollisionManager::CheckPlayerCollisionY(Player& player, const sf::Vector2f& nextPosition, const TileMap& map) const
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
                }

            }
        }
    }

    return isColliding;
}

const bool CollisionManager::CheckBulletCollisionWithEnemies(const Bullet& bullet, std::vector<Enemy>& enemies) const
{
    bool isColliding = false;
    for (Enemy& enemy : enemies) 
    {
        if (bullet.IsColliding(enemy)) 
        {
            enemy.Damage(); // TODO : use events ?
            isColliding = true;
        }
    }

    return isColliding;
}

const bool CollisionManager::CheckBulletCollisionWithMap(const Bullet& bullet, const TileMap& map) const
{
    for (auto& tile : map.GetTileMap())
    {
        if (std::shared_ptr<CollideableTile> t = std::dynamic_pointer_cast<CollideableTile>(tile))
        {
            if (t->Contains(bullet.GetCenter()))
            {
                return true;
            }
        }
    }

    return false;
}