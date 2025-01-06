#include "GamePCH.h"
#include "BlockMesh.h"

const std::array<std::array<BlockVertex, 6>, 6> BlockMesh::vertices = {
	{
		{
			{   // Top
				{{0, 1, 1}, {1, 0}},
				{{1, 1, 1}, {1, 1}},
				{{0, 1, 0}, {0, 0}},
				{{1, 1, 1}, {1, 1}},
				{{1, 1, 0}, {0, 1}},
				{{0, 1, 0}, {0, 0}},
			}
		},
		{
			{   // +X East
				{{1, 1, 1}, {1, 0}},
                {{1, 0, 1}, {1, 1}},
                {{1, 1, 0}, {0, 0}},
                {{1, 0, 1}, {1, 1}},
                {{1, 0, 0}, {0, 1}},
                {{1, 1, 0}, {0, 0}},
			}
		},
		{
			{   //-X West
				{{0, 1, 0}, {1, 0}},
				{{0, 0, 0}, {1, 1}},
				{{0, 1, 1}, {0, 0}},
				{{0, 0, 0}, {1, 1}},
				{{0, 0, 1}, {0, 1}},
				{{0, 1, 1}, {0, 0}},
			}
		},
		{
			{   //-Z North
				{{1, 1, 0}, {1, 0}},
				{{1, 0, 0}, {1, 1}},
				{{0, 1, 0}, {0, 0}},
				{{1, 0, 0}, {1, 1}},
				{{0, 0, 0}, {0, 1}},
				{{0, 1, 0}, {0, 0}},
			}
		},
		{
			{		// +Z South
				{{0, 1, 1}, {1, 0}},
				{{0, 0, 1}, {1, 1}},
				{{1, 1, 1}, {0, 0}},
				{{0, 0, 1}, {1, 1}},
				{{1, 0, 1}, {0, 1}},
				{{1, 1, 1}, {0, 0}},
			}
		},
		{
			{		// Bottom
				{{1, 0, 1}, {1, 0}},
				{{0, 0, 1}, {1, 1}},
				{{1, 0, 0}, {0, 0}},
				{{0, 0, 1}, {1, 1}},
				{{0, 0, 0}, {0, 1}},
				{{1, 0, 0}, {0, 0}},
			}
		},
	}
};