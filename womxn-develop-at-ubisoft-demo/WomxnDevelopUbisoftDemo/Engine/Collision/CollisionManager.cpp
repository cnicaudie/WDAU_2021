#include <stdafx.h>
#include "CollisionManager.h"

#include <Game/Map/CollideableTile.h>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckCollision(BoxCollideable* first, const sf::Vector2f& positionOffset, const MapGrid& mapGrid) const
{
    bool hasCollided = false;
    
    // Get the bounding boxes for the previous and the next position
    sf::FloatRect startCollider = first->GetBoundingBox();
    sf::FloatRect endCollider = startCollider;
    endCollider.left += positionOffset.x;
    endCollider.top += positionOffset.y;
    
    // Define the starting and end points of the first collider bounding box 
    sf::Vector2f startPointTop(startCollider.left, startCollider.top);
    sf::Vector2f startPointBottom(startCollider.left + startCollider.width, startCollider.top + startCollider.height);
    sf::Vector2f endPointTop(endCollider.left, endCollider.top);
    sf::Vector2f endPointBottom(endCollider.left + endCollider.width, endCollider.top + endCollider.height);
    
    // Draw a quad between the previous and next positions
    sf::ConvexShape quad(4);
    quad.setPoint(0, startPointTop);
    quad.setPoint(1, endPointTop);
    quad.setPoint(2, endPointBottom);
    quad.setPoint(3, startPointBottom);
    
    // Get its bounding box
    sf::FloatRect quadBoundingBox = quad.getGlobalBounds();
    //std::cout << "Quad top left position : " << quadBoundingBox.left << " / " << quadBoundingBox.top << std::endl;
    //std::cout << "Quad width height : " << quadBoundingBox.width << " / " << quadBoundingBox.height << std::endl;

    std::vector<std::shared_ptr<Tile>> tiles = mapGrid.GetBoundingTiles(quadBoundingBox);
    //std::cout << "Number of tiles to check : " << tiles.size() << std::endl;

    for (std::shared_ptr<Tile>& t : tiles)
    {
        if (t->IsTrigger()) 
        {
            for (const std::shared_ptr<BoxCollideable>& b : t->GetCollideablesOnTile()) 
            {
                // Check collision with collideables in tile
                if (quadBoundingBox.intersects(b->GetBoundingBox())) 
                {
                    hasCollided = true;

                    // If collision detected, call "OnCollision"
                    first->OnCollision(b);
                    //b->OnCollision(first);
                }
            }
        }
        // Check collision with tile itself (if collideable)
        else if (quadBoundingBox.intersects(t->GetBoundingBox())) 
        {
            hasCollided = true;

            // If collision detected, call "OnCollision"
            //std::cout << "Collided with tile" << std::endl;
            first->OnCollision(t);
            //t.OnCollision(*first);
            
        }
    }

    //std::cout << "Ended collision check" << std::endl;
    return hasCollided;
}

/*

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

const bool CollisionManager::CheckBulletCollisionWithMap(const Bullet& bullet, const Map& map) const
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
*/