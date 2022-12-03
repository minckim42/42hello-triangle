#include <iostream>
#include "glfw_util.h"
#include "shader.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "assimp_loader.h"
#include "camera.h"

using namespace std;

struct WindowInfo
{
   glm::vec2 window_size;
   glm::vec2 prev_cursor_position;
   glm::vec2 curr_cursor_position;
   Camera* camera;
};

void key_manager(GLFWwindow *window, Camera& camera)
{
   WindowInfo& window_info = *static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
   glm::vec2 mouse_motion = (window_info.curr_cursor_position - window_info.prev_cursor_position) / window_info.window_size;
   float move_velocity = 0.1f;
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, true);
   }
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
   {
      camera.move_forward(move_velocity);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
   {
      camera.move_backward(move_velocity);
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
   {
      camera.move_left(move_velocity);
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
   {
      camera.move_right(move_velocity);
   }
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
   {
      camera.pitch_down(mouse_motion.y);
      camera.turn_right(mouse_motion.x);
   }
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
   return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

int main_process()
{
   WindowInfo window_info = {
      .window_size = {1280, 720},
      .prev_cursor_position = {0.0f, 0.0f},
      .curr_cursor_position = {0.0f, 0.0f}
   };

   // GLFW 윈도우 초기화
   GLFWwindow* window = init_window(window_info.window_size.x, window_info.window_size.y, "model loading test");

   // window 객체에 사용자가 정의한 구조체에 접근할 수 있는 포인터를 지정
   glfwSetWindowUserPointer(window, &window_info);

   // 프레임버퍼 콜백 함수 등록
   glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h){
      // 콜백함수에서 이런 방식으로 외부 변수에 접근할 수 있음!
      WindowInfo& window_info = *static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
      window_info.window_size = {w, h};
      glViewport(0, 0, w, h);
   });

   // 마우스 휠 콜백 함수 등록
   glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
      WindowInfo& window_info = *static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
      Camera& camera = *window_info.camera;
      float move_velocity = 2.0f;
      if (yoffset == 1)
      {
         camera.move_up(move_velocity);
      }
      else if (yoffset == -1)
      {
         camera.move_down(move_velocity);
      }
      camera.update();
   });

   // 셰이더
   GLuint shader_program = compile_shader(
      ROOT_PATH"/source/basic.vert",
      ROOT_PATH"/source/basic.frag"
   );

   // 모델
   Texture::Container texture_container;
   Material::Container material_container;
   Mesh::Container mesh_container;
   Model::Ptr model = assimp_load(
      ROOT_PATH"/resource/backpack/backpack.obj",
      texture_container,
      material_container,
      mesh_container
   );

   // 카메라 세팅
   std::pair<glm::vec3, glm::vec3> bounding_box = model->get_bounding_box(glm::mat4(1.0f));
   glm::vec3 center = (bounding_box.first + bounding_box.second) * 0.5f;
   glm::vec3 size = bounding_box.second - bounding_box.first;
   float far = glm::length(size) * 100.0f;
   float near = far * 0.0001f;
   constexpr glm::vec3 unit_z = {0.0f, 0.0f, 1.0f};

   Camera camera(
      center + size * 3.0f,
      -size,
      unit_z,
      window_info.window_size,
      near,
      far,
      glm::pi<float>() * 0.333333f
   );
   window_info.camera = &camera;

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   // 렌더링 루프
   while (!glfwWindowShouldClose(window))
   {
      // 마우스 위치
      double mouse_pos[2];
      glfwGetCursorPos(window, mouse_pos, mouse_pos + 1);
      window_info.curr_cursor_position = {mouse_pos[0], mouse_pos[1]};

      // 버퍼 초기화
      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // 키와 마우스 동작
      key_manager(window, camera);

      // 그리기
      glUseProgram(shader_program);
      set_uniform(shader_program, "eye", camera.position);
      model->draw(shader_program, glm::mat4(1.0f), camera.view, camera.projection);

      // 카메라
      camera.screen_size = window_info.window_size;
      camera.update();
      window_info.prev_cursor_position = window_info.curr_cursor_position;

      // 창 관리
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glDeleteProgram(shader_program);

   return 0;
}

int main()
{
   try
   {
      int result = main_process();
      glfwTerminate();
      return result;
   }
   catch(const string& err)
   {
      std::cerr << err << '\n';
      glfwTerminate();
      return -1;
   }
}
