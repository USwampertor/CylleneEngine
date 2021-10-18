#version 330 core

uniform mat4 View;
uniform mat4 Projection;

in vec2 vUVOut;
in vec3 vColorOut;

uniform sampler2D ourTexture;

out vec4 LFragment;

void
main() {
  //LFragment = vec4(vUVOut.xy, 0.0, 1.0);
  //LFragment = vec4(vColorOut, 1.0);
  //LFragment = vec4(vUVOut.xy, min(0.0, vColorOut.x), 1.0);
  LFragment = vec4(texture(ourTexture, vUVOut).rgb, 1.0f) + vec4(vColorOut, 1.0f);
  //LFragment = vec4(texture(ourTexture, vUVOut).rgb, 1.0f);
}