#include "camera.h"

Camera::Camera(
   const glm::vec3& position,
   const glm::vec3& direction,
   const glm::vec3& up,
   const glm::vec2& screen_size,
   float near,
   float far,
   float fovy
):
   Move(position, direction, up),
   screen_size(screen_size),
   near(near),
   far(far),
   fovy(fovy)
{
   update();
}

void Camera::update() noexcept
{
   view = glm::lookAt(position, position + direction, up);
   projection = glm::perspective(fovy, screen_size.x / screen_size.y, near, far);
}