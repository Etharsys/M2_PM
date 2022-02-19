#version 300 es
precision mediump float;

in vec3 vNormal_vs;

uniform mat3 uColor;

out vec3 fFragColor;

void main() {
  fFragColor = vec3(uColor);
};
