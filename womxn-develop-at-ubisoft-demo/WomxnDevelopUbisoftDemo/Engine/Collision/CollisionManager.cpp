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
    
    std::vector<std::shared_ptr<Tile>> tiles = mapGrid.GetBoundingTiles(quadBoundingBox);
    
    for (std::shared_ptr<Tile>& t : tiles)
    {
        if (t->IsTrigger()) 
        {
            for (BoxCollideable* b : t->GetCollideablesOnTile()) 
            {
                // Check collision with collideables in tile
                if (b->Contains(first->GetCenter())) 
                {
                    if (b->IsTrigger()) 
                    {
                        b->OnTrigger(first);
                    } 
                    else 
                    {
                        hasCollided = true;
                        //first->OnCollision(b);
                        //b->OnCollision(first);

                    }
                }
            }
        }
        // Check collision with tile itself (if collideable)
        else if (quadBoundingBox.intersects(t->GetBoundingBox())) 
        {
            hasCollided = true;

            first->OnCollision(static_cast<const BoxCollideable*>(t.get()));
            //t.OnCollision(*first);    
        }
    }
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
*/