#version 330 core

uniform mat4 View;
uniform mat4 Projection;

in vec4 texCoord;
in vec4 frontColor;

out vec4 FragColor;

void
main() {
  FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}