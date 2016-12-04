#version 330 core

uniform float i_time;

in vec2 vertTexCoord0;

out vec4 fragColor;

void main() {
  vec2 pos = 2 * (vertTexCoord0 - vec2(0.5));
  fragColor = vec4(vec3(sin(length(pos) * 20 * i_time)), 1);
}
