#include "GamePCH.h"
#include "Player.h"
#include "Math/WorldRayCast.h"
#include "World/Persistence.h"

bool MovementSimulation::CanMove(const glm::vec3 &from, const glm::vec3 &to, World &world)
{
	glm::vec3 movementDirection = from - to;

	AxisAlignedBoundingBox playerAABBTemp = Player::PlayerAABB;
	playerAABBTemp.minPoint += to;
	playerAABBTemp.maxPoint += to;

	for (auto point : Player::PlayerBoundingBox)
	{
		point += to;

		// Ray-casting to determine which blocks should be used in collision calculation
		if (WorldRayCast ray{ point, movementDirection, world, 3.0f })
		{
			glm::vec3 position = ray.GetHitTarget().position;
			if (AxisAlignedBoundingBox::PositionFromBlock(position).Intersect(playerAABBTemp)) return false;
		}
	}
	return true;
}

Player::Player(const std::shared_ptr<World>& world, const std::shared_ptr<Persistence>& persistence)
	: camera(persistence->GetCamera()), persistence(persistence), world(world) {}

Player::~Player()
{
	persistence->CommitCamera(camera);
}

void Player::Update(float deltaTime)
{
	gravity += glm::vec3(0, -1, 0) * GravityConstant * deltaTime;
	glm::vec3 moveDirection = camera.GetMoveDirection();

	CanJump = false;
	glm::vec3 movement(0);
	if (glm::length(moveDirection) > 0) 
	{
		float movementSpeed = isRunning ? GetRunningSpeed() : GetWalkingSpeed();
		movement = glm::normalize(moveDirection) * movementSpeed * deltaTime;
	}

	glm::vec3 position = camera.GetPosition();

	if (isSurvivalMovement) 
	{
		std::array<glm::vec3, 3> axes = 
		{
			{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			}
		};

		for (const auto& axis : axes) 
		{
			glm::vec3 movementInAxis = movement * axis;
			if (MovementSimulation::CanMove(position, position + movementInAxis, *world)) 
			{
				position += movementInAxis;
			}
		}

		glm::vec3 positionWithGravity = position + gravity * deltaTime;
		if (MovementSimulation::CanMove(position, positionWithGravity, *world)) 
		{
			position = positionWithGravity;
		}
		else 
		{
			CanJump = true;
			gravity = glm::vec3(0);
		}

	}
	else 
	{
		position += movement;
	}

	camera.SetPosition(position);
}

void Player::OnKeyEvent(int32_t key, int32_t, int32_t action, int32_t)
{
	if (action == GLFW_REPEAT) return;

	bool isButtonPressed = action == GLFW_PRESS;

	if (key == GLFW_KEY_W) camera.SetMovingForward(isButtonPressed);
	else if (key == GLFW_KEY_S) camera.SetMovingBackward(isButtonPressed);
	else if (key == GLFW_KEY_A) camera.SetMovingLeft(isButtonPressed);
	else if (key == GLFW_KEY_D) camera.SetMovingRight(isButtonPressed);
	else if (key == GLFW_KEY_SPACE) 
	{
		if (isSurvivalMovement) 
		{
			camera.SetMovingUp(false);
			if (CanJump && isButtonPressed) 
			{
				gravity = glm::vec3(0, GetJumpSpeed(), 0);
			}
		}
		else 
		{
			camera.SetMovingUp(isButtonPressed);
		}
	}
	else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) 
	{
		isRunning = isButtonPressed;
	}
	else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) 
	{
		if (isSurvivalMovement) camera.SetMovingDown(false);		
		else camera.SetMovingDown(isButtonPressed);
	}
}

void Player::OnMouseButtonEvent(int32_t button, int32_t action, int32_t)
{
	if (action != GLFW_PRESS) return;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (WorldRayCast ray{ camera.GetPosition(), camera.GetLookDirection(), *world, Reach }) 
		{
			world->PlaceBlock(BlockData::BlockType::Air, ray.GetHitTarget().position);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) 
	{
		WorldRayCast ray{ camera.GetPosition(), camera.GetLookDirection(), *world, Reach };
		if (ray && ray.GetHitTarget().hasNeighbor) 
		{
			world->PlaceBlock(BlockToPlace, ray.GetHitTarget().neighbor);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) 
	{
		if (WorldRayCast ray{ camera.GetPosition(), camera.GetLookDirection(), *world, Reach }) 
		{
			BlockToPlace = ray.GetHitTarget().block.type;
		}
	}
}

void Player::OnCursorPositionEvent(double x, double y)
{
	static double lastX = x;
	static double lastY = y;

	if (shouldResetMouse) 
	{
		shouldResetMouse = false;
		lastX = x;
		lastY = y;
	}

	float yaw = camera.GetYaw() + static_cast<float>(-lastX + x) * MouseSensitivity;
	float pitch = glm::clamp(camera.GetPitch() + static_cast<float>(lastY - y) * MouseSensitivity, -89.0f, 89.0f);
	camera.UpdateCameraOrientation(yaw, pitch);

	lastX = x;
	lastY = y;
}

void Player::ResetMousePosition()
{
	shouldResetMouse = true;
}