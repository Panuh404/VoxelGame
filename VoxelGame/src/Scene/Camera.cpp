#include "GamePCH.h"
#include "Camera.h"

const glm::mat4& Camera::UpdateView()
{
    return view = CalculateView();
}

const glm::mat4& Camera::LookAt(glm::vec3 eye, glm::vec3 center)
{
    position = eye;
    UpdateCameraDirection(center);
    return UpdateView();
}

const glm::mat4& Camera::SetPosition(glm::vec3 eye)
{
    position = eye;
    return UpdateView();
}

void Camera::UpdateCameraDirection(glm::vec3 newForward)
{
    lookDirection = newForward;
    newForward.y = 0;
    forward.direction = glm::normalize(newForward);
    backward.direction = -newForward;

    right.direction = glm::normalize(glm::cross(newForward, cameraUp));
    left.direction = -right.direction;
}

void Camera::UpdateCameraOrientation(float newYaw, float newPitch)
{
    yaw = newYaw;
    pitch = newPitch;
    UpdateCameraDirection(glm::normalize(glm::vec3
        {
	       glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
	       glm::sin(glm::radians(pitch)),
	       glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        }));

    UpdateView();
}

glm::vec3 Camera::GetPosition() const
{
    return position;
}

glm::mat4 Camera::CalculateView() const
{
    return glm::lookAt(position, position + lookDirection, cameraUp);
}

glm::vec3 Camera::GetLookDirection() const
{
    return lookDirection;
}
glm::vec3 Camera::GetMoveDirection()
{
    auto moveDirection = glm::vec3(0);

    std::array<MovementDirection*, 6> directions = 
    {
    	&forward,
    	&backward,
    	&left,
    	&right,
    	&up,
    	&down,
    };

    for (const auto direction : directions)
    {
        if (!direction->isMoving) continue;
        moveDirection += direction->direction;
    }

    return moveDirection;
}
