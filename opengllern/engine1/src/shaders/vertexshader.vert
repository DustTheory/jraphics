#version 150

in vec3 vertex_position;
in vec3 vertex_color;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  color = vertex_color;
  gl_Position = proj * view * model * vec4(vertex_position, 1.0f);
}