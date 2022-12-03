#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texture_coord;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_color;
out vec2 v_texture_coord;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	v_position = position;
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_color = color;
	v_texture_coord = texture_coord;
	v_normal = normal;
}