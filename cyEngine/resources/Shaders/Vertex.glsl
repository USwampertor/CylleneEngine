#version 330 core

uniform mat4 View;
uniform mat4 Projection;

layout (location = 0) in vec3 LVertexPos;
layout (location = 1) in vec2 LVertexUV;
layout (location = 2) in vec3 LVertexColor;

out vec4 texCoord;
out vec4 frontColor;

void
main() {
    gl_Position = Projection * View * vec4(LVertexPos, 1.0);
    texCoord = vec4(LVertexUV, 0.0, 0.0);
    frontColor = vec4(LVertexColor, 0.0);
}