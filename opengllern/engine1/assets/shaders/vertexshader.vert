#version 150

in vec3 vertex_position;
in vec2 vertex_texture_coordinates;

out vec2 texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  texture_coordinates = vertex_texture_coordinates;
  gl_Position = proj * view * model * vec4(vertex_position, 1.0f);
}