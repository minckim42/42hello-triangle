#include "move.h"


Move::Move(glm::vec3 position, glm::vec3 direction, glm::vec3 up):
   position(position),
   direction(glm::normalize(direction)),
   up(glm::normalize(up)),
   right(glm::normalize(glm::cross(direction, up)))
{}

void Move::move_forward(float val) noexcept
{  position += direction * val;  }

void Move::move_backward(float val) noexcept
{  position -= direction * val;  }

void Move::move_left(float val) noexcept
{  position -= right * val;   }

void Move::move_right(float val) noexcept
{  position += right * val;   }

void Move::move_up(float val) noexcept
{  position += up * val;   }

void Move::move_down(float val) noexcept
{  position -= up * val;   }

void Move::pitch_up(float rad) noexcept
{
   constexpr float limit = 0.05f;
   if (rad > 0.0f && glm::dot(direction, up) > std::cos(rad) - limit)
   {
      return;
   }
   if (rad < 0.0f && glm::dot(direction, up) < std::cos(3.141592 + rad) + limit)
   {
      return;
   }
   direction = rotate(direction, right, rad);
}

void Move::pitch_down(float rad) noexcept
{
   pitch_up(-rad);
}

void Move::turn_left(float rad) noexcept
{
   right = rotate(right, up, rad);
   direction = rotate(direction, up, rad);
}

void Move::turn_right(float rad) noexcept
{
   right = rotate(right, up, -rad);
   direction = rotate(direction, up, -rad);
}

glm::vec3 rotate(const glm::vec3& input, glm::vec3 axis, float rad) noexcept
{
   return glm::quat(std::cos(rad), glm::normalize(axis) * std::sin(rad)) * input;
}