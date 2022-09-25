#version 460 core

in vec3 v_color;
in vec2 v_texture_coord;
out vec4 FragColor;

uniform sampler2D texture_id;
uniform float foo;

void main()
{
	FragColor = mix(
		texture(texture_id, v_texture_coord),
		// vec4(0.0f),
		vec4(v_color, 1.0f),
		foo * foo
	);
}