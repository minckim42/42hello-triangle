#pragma once
#include "move.h"

class Camera: public Move
{
public:

   glm::mat4 view;
   glm::mat4 projection;
   glm::vec2 screen_size;
   float near;
   float far;
   float fovy;

   Camera(
      const glm::vec3& position,
      const glm::vec3& direction,
      const glm::vec3& up,
      const glm::vec2& screen_size,
      float near,
      float far,
      float fovy
   );
   void update() noexcept;
};