#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;
in vec2 v_texture_coord;
out vec4 FragColor;

layout (location = 0) uniform sampler2D ambient;
layout (location = 1) uniform sampler2D diffuse;
layout (location = 2) uniform sampler2D specular;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform vec3 eye;

const vec3 light_position = vec3(1, 1, 1) * 50.0f;
const float light_strength = 40000.0f;

const float zero = 0.0f;
const float one = 1.0f;


void main()
{
	const vec3 light_direction = light_position - v_position;
	const float light_distance_sqare = dot(light_direction, light_direction);
	const vec3 normalized_light_direction = normalize(light_direction);
	const vec3 eye_from_position = v_position - eye;

	vec4 ambient_color = texture(ambient, v_texture_coord);
	ambient_color.rgb *= ka;

	vec4 diffuse_color = texture(diffuse, v_texture_coord);
	float diffuse_intensity = clamp(dot(normalized_light_direction, v_normal), zero, one);
	diffuse_intensity *= light_strength / light_distance_sqare;
	diffuse_color.rgb *= diffuse_intensity;
	diffuse_color.rgb *= kd;


	vec4 specular_color = texture(specular, v_texture_coord);
	float specular_intensity = clamp(dot(eye_from_position, reflect(normalized_light_direction, v_normal)), zero, one);
	specular_intensity *= light_strength / light_distance_sqare;
	specular_color.rgb *= specular_intensity;
	specular_color.rgb *= ks;

	FragColor = clamp((ambient_color + diffuse_color + specular_color) * 0.3333f, zero, one);
}