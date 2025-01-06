#pragma once

struct MovementDirection {
	bool isMoving = false;
	glm::vec3 direction = glm::vec3(0);
};

class Camera {
	glm::mat4 view = CalculateView();
	glm::vec3 position = { 14, 100, 17 };
	glm::vec3 cameraUp = { 0, 1, 0 };

	MovementDirection forward = { false, {1, 0, 0} };
	MovementDirection backward = { false, {-1, 0, 0} };
	MovementDirection left = { false, {0, 0, 1} };
	MovementDirection right = { false, {0, 0, -1} };
	MovementDirection up = { false, {0, 1, 0} };
	MovementDirection down = { false, {0, -1, 0} };
	glm::vec3 lookDirection = forward.direction;

	float yaw = 0;
	float pitch = 0.5;

	glm::mat4 CalculateView() const;
	const glm::mat4& UpdateView();


public:
	const glm::mat4& LookAt(glm::vec3 eye, glm::vec3 center);

	void UpdateCameraDirection(glm::vec3 newForward);
	void UpdateCameraOrientation(float yaw, float pitch);

	const glm::mat4& GetViewMatrix() const { return view; }
	float GetYaw() const { return yaw; };
	float GetPitch() const { return pitch; };

	const glm::mat4& SetPosition(glm::vec3 eye);
	glm::vec3 GetLookDirection() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetMoveDirection();

	void SetMovingForward(bool isMoving) { forward.isMoving = isMoving; }
	void SetMovingBackward(bool isMoving) { backward.isMoving = isMoving; }
	void SetMovingLeft(bool isMoving) { left.isMoving = isMoving; }
	void SetMovingRight(bool isMoving) { right.isMoving = isMoving; }
	void SetMovingUp(bool isMoving) { up.isMoving = isMoving; }
	void SetMovingDown(bool isMoving) { down.isMoving = isMoving; }
};
