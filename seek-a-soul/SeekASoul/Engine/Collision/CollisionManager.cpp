#include <stdafx.h>
#include "CollisionManager.h"
#include <Game/Map/Tiles/CollideableTile.h>

CollisionManager::CollisionManager() {}

const bool CollisionManager::CheckCollision(BoxCollideable* collideable, const sf::Vector2f& positionOffset, const MapGrid& mapGrid) const
{
    bool hasCollided = false;
    
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
                //tile->OnTrigger(collideable);
            }

            for (BoxCollideable* otherCollideable : tile->GetCollideablesOnTile()) 
            {
                // Check collision with collideables on tile
                if (otherCollideable->IsTrigger() && collideable->Contains(otherCollideable->GetCenter()))
                {
                    collideable->OnTrigger(static_cast<BoxCollideable*>(otherCollideable));
                    otherCollideable->OnTrigger(collideable);
                } 
                else if (collideable->IsColliding(*otherCollideable))
                {
                    //hasCollided = true;
                    collideable->OnCollision(static_cast<BoxCollideable*>(otherCollideable));
                    otherCollideable->OnCollision(collideable);
                }
            }
        }
        // Check collision with tile itself
        else if (quadBoundingBox.intersects(tile->GetBoundingBox())) 
        {
            hasCollided = true;
            collideable->OnCollision(static_cast<BoxCollideable*>(tile.get()));
            //tile->OnCollision(collideable);
        }
    }
    return hasCollided;
}