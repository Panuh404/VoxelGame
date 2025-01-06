#include "GamePCH.h"
#include "CubeMesh.h"

#include "BlockMesh.h"
#include "BlockVertex.h"

CubeMesh::CubeMesh()
{
    std::vector<BlockVertex> vertices;
    vertices.resize(6 * 6);

    int32_t vertexCount = 0;
    for (const auto& face : BlockMesh::vertices) {
        for (const auto& vertex : face) {
            vertices.at(vertexCount) = vertex;
            vertexCount++;
        }
    }
    cubeVertexArray = std::make_shared<VertexArray>(vertices, BlockVertex::VertexAttributes());
}

void CubeMesh::Render() const
{
	cubeVertexArray->RenderVertexStream();
}
