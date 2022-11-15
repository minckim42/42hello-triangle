#version 460 core

in vec3 v_color;
in vec2 v_texture_coord;
out vec4 FragColor;

uniform sampler2D input_texture;
uniform float foo;

void main()
{
	const vec4 texture_color = texture(input_texture, v_texture_coord);
	const vec4 vertex_color = vec4(v_color, 1.0f);
	const float t = foo * foo;

	FragColor = mix(texture_color, vertex_color, t);
}