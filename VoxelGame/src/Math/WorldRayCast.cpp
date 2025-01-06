#include "GamePCH.h"
#include "WorldRayCast.h"
#include "AxisPlane.h"

WorldRayCast::WorldRayCast(glm::vec3 position, glm::vec3 direction, World& world, float reach = 10.0f)
{
    success = false;

    std::array<AxisPlane, 3> planes = 
    {
    	{
    		AxisPlane{{1, 0, 0}, position, direction},
    		AxisPlane{{0, 1, 0}, position, direction},
    		AxisPlane{{0, 0, 1}, position, direction},
    	}
    };

    std::sort(planes.begin(), planes.end());
    glm::vec3 prevClosestHits[2] = { position, position };  // the player might be inside a block
    bool hasNeighbor = false;

    while (!success && planes[0].GetHitDistance() <= reach) 
{
        std::optional<glm::ivec3> maybeBlockPosition = AxisPlane::RayHitsToBlockPosition(planes[0].GetHitPosition(), prevClosestHits[1]);

        if (maybeBlockPosition.has_value() && World::IsValidBlockPosition(maybeBlockPosition.value())) 
        {
            glm::vec3 blockPosition = maybeBlockPosition.value();
            const BlockData* block = world.GetBlockAt(blockPosition);

            if (block->type != BlockData::BlockType::Air) 
            {
                success = true;
                std::optional<glm::ivec3> maybeNeighbor = AxisPlane::RayHitsToBlockPosition(prevClosestHits[0], prevClosestHits[1]);

                hitTarget = { blockPosition, *block, maybeNeighbor.value(), hasNeighbor && maybeNeighbor.has_value() };
            }
        }

        hasNeighbor = true;
        prevClosestHits[0] = prevClosestHits[1];
        prevClosestHits[1] = planes[0].GetHitPosition();
        planes[0].AdvanceOffset();
        std::sort(planes.begin(), planes.end());
    }
}

