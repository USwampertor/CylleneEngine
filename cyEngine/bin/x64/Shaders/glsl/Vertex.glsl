#version 330 core

uniform mat4 View;
uniform mat4 Projection;

in vec3 LVertexPos;
in vec2 LVertexUV;
in vec3 LVertexColor;

out vec2 vUVOut;
out vec3 vColorOut;

void
main() {
    vec4 vertex = (Projection * View) * vec4(LVertexPos, 1.0f);
    vUVOut = LVertexUV;
    vColorOut = LVertexColor;
    //gl_Position = vec4(LVertexPos.x, LVertexPos.y, 0, 1);
    gl_Position = vertex;
}