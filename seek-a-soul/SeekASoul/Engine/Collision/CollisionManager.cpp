#include <stdafx.h>
#include "CollisionManager.h"
#include <Game/Map/Tiles/CollideableTile.h>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset, const MapGrid& mapGrid) const
{
    bool hasCollidedWithTile = false;
    
    // Get the bounding boxes for the previous and the next position
    sf::FloatRect startCollider = collideable->GetBoundingBox();
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
    
    for (std::shared_ptr<Tile>& tile : tiles)
    {
        if (tile->IsTrigger()) 
        {
            if (tile->Contains(collideable->GetCenter()))
            {
                collideable->OnTrigger(static_cast<BoxCollideable*>(tile.get()));
                // Uncomment next line when implementing portal tiles or whatever
                //tile->OnTrigger(collideable);
            }

            // Check collision with collideables on tile
            for (BoxCollideable* otherCollideable : tile->GetCollideablesOnTile()) 
            {
                if (otherCollideable->IsTrigger() && collideable->Contains(otherCollideable->GetCenter()))
                {
                    collideable->OnTrigger(otherCollideable);
                    otherCollideable->OnTrigger(collideable);
                } 
                else if (collideable->IsColliding(*otherCollideable))
                {
                    collideable->OnCollision(otherCollideable, GetCollisionDirection(collideable, otherCollideable));
                    otherCollideable->OnCollision(collideable, GetCollisionDirection(otherCollideable, collideable));
                }
            }
        }
        // Check collision with tile itself
        else if (quadBoundingBox.intersects(tile->GetBoundingBox())) 
        {
            hasCollidedWithTile = true;
            collideable->OnCollision(static_cast<BoxCollideable*>(tile.get()), GetCollisionDirection(collideable, tile.get()));
            // Uncomment next line when implementing breakable tiles or whatever
            //tile->OnCollision(collideable, GetCollisionDirection(tile.get(), collideable));
        }
    }

    return hasCollidedWithTile;
}

const CollisionDirection CollisionManager::GetCollisionDirection(BoxCollideable* boxCollideable, BoxCollideable* boxCollider) const
{
    sf::FloatRect collideable = boxCollideable->GetBoundingBox();
    sf::FloatRect collider = boxCollider->GetBoundingBox();

    int32_t collisionDirection = static_cast<int32_t>(CollisionDirection::NONE);

    // === Compute collision direction for anticipative collision (e.g Player will collide with Tile)

    if (collideable.top + collideable.height <= collider.top
        && collideable.top < collider.top)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::BOTTOM);
    }
    else if (collideable.top >= collider.top + collider.height
        && collideable.top + collideable.height > collider.top + collider.height)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::TOP);
    }
    else if (collideable.left >= collider.left + collider.width
        && collideable.left + collideable.width > collider.left + collider.width)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::LEFT);
    }
    else if (collideable.left + collideable.width <= collider.left
        && collideable.left < collider.left)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::RIGHT);
    }

    // === Compute collision direction for direct collision (e.g Player is colliding with Tile)

    else if (collideable.top + collideable.height > collider.top
        && collideable.top < collider.top)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::BOTTOM);
        collisionDirection |= static_cast<int32_t>(CollisionDirection::IN_BOTTOM);
    }
    else if (collideable.top < collideable.top + collider.height
        && collideable.top > collider.top)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::TOP);
        collisionDirection |= static_cast<int32_t>(CollisionDirection::IN_TOP);
    }
    else if (collideable.left < collider.left + collider.width
        && collideable.left + collideable.width > collider.left + collider.width)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::LEFT);
        collisionDirection |= static_cast<int32_t>(CollisionDirection::IN_LEFT);
    }
    else if (collideable.left + collideable.width > collider.left
        && collideable.left < collider.left)
    {
        collisionDirection |= static_cast<int32_t>(CollisionDirection::RIGHT);
        collisionDirection |= static_cast<int32_t>(CollisionDirection::IN_RIGHT);
    }
    
    return static_cast<CollisionDirection>(collisionDirection);
}