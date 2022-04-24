#version 150

in vec2 texture_coordinates;
out vec4 frag_color;

uniform sampler2D g_sampler;

void main() {
  //texture2D(g_sampler, texture_coordinates)[0]
  frag_color = texture2D(g_sampler, texture_coordinates);
}