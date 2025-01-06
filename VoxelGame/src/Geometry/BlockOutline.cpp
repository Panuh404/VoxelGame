#include "GamePCH.h"
#include "BlockOutline.h"

#include "GameSettings.h"
#include "Scene/AssetManager/AssetManager.h"

BlockOutline::BlockOutline(std::shared_ptr<const CubeMesh> blockMesh)
	:	outlinedShader(AssetManager:: instance().loadShaderProgram(Settings::ShaderOutline)),
		blockMesh(std::move(blockMesh)) {}

void BlockOutline::Render(const glm::mat4& transform) const
{
	outlinedShader->SetMat4("MVP", transform);
	outlinedShader->Bind();
	blockMesh->Render();
}
