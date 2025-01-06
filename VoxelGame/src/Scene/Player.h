#pragma once
#include "Camera.h"
#include "GameSettings.h"
#include "Geometry/BlockData.h"
#include "Math/AxisAlignedBoundingBox.h"
#include "World/World.h"


namespace MovementSimulation
{
	bool CanMove(const glm::vec3 &from, const glm::vec3 &to, World &world);
}

class Player
{
	Camera camera;
	std::shared_ptr<World> world;
	std::shared_ptr<Persistence> persistence;
	BlockData::BlockType BlockToPlace = BlockData::BlockType::Grass;

	glm::vec3 gravity{ 0 };

	float MovementSpeedMultiplier = Settings::MovementSpeedMultiplier;
	float MouseSensitivity = Settings::MouseSensitivity;
	float GravityConstant = DefaultGravity;
	float JumpHeightMultiplier = Settings::JumpHeightMultiplier;

	bool CanJump = false;
	bool isRunning = false;
	bool isSurvivalMovement = Settings::isSurvivalMovement;
	bool shouldResetMouse = true;

public:
	explicit Player(const std::shared_ptr<World>& world, const std::shared_ptr<Persistence>& persistence);
	~Player();

	static constexpr AxisAlignedBoundingBox PlayerAABB = AxisAlignedBoundingBox
	{
	 {-0.3, -1.5, -0.3},
	 {0.3, 0.3, 0.3},
	};

	static constexpr std::array<glm::vec3, 8> PlayerBoundingBox = {
		{
			{0.3, 0.3, 0.3},
			{0.3, 0.3, -0.3},
			{-0.3, 0.3, 0.3},
			{-0.3, 0.3, -0.3},
			{0.3, -1.5, 0.3},
			{0.3, -1.5, -0.3},
			{-0.3, -1.5, 0.3},
			{-0.3, -1.5, -0.3},
		}
	};

	static constexpr float DefaultGravity = 46.62f;
	static constexpr float Reach = 4.5f;

	void Update(float deltaTime);

	bool GetSurvivalMovement() const { return isSurvivalMovement; }
	void SetSurvivalMovement(bool isSurvival)
	{
		gravity = glm::vec3(0);
		isSurvivalMovement = isSurvival;
	}

	const Camera& GetCamera() const { return camera; }

	float GetJumpHeightMultiplier() const { return JumpHeightMultiplier; }
	float GetGravityConstant() const { return GravityConstant; }
	float GetMovementSpeedMultiplier() const { return MovementSpeedMultiplier; }
	float GetJumpSpeed() const { return JumpHeightMultiplier * GravityConstant / 4.5f; }
	float GetWalkingSpeed() const { return MovementSpeedMultiplier * 4.317f; }
	float GetRunningSpeed() const { return MovementSpeedMultiplier * 5.612f; }
	BlockData::BlockType GetBlockToPlace() const { return BlockToPlace; }

	void SetBlockToPlace(BlockData::BlockType block) { BlockToPlace = block; }
	void SetJumpHeightMultiplier(float multiplier) { JumpHeightMultiplier = multiplier; }
	void SetGravityConstant(float constant) { GravityConstant = constant; }
	void SetMovementSpeedMultiplier(float movementSpeed) { MovementSpeedMultiplier = movementSpeed; }

	void OnKeyEvent(int32_t key, int32_t scancode, int32_t action, int32_t mode);
	void OnMouseButtonEvent(int32_t button, int32_t action, int32_t mods);
	void OnCursorPositionEvent(double d, double d1);

	void ResetMousePosition();
};

