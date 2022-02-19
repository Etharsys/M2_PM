#version 300 es
precision mediump float;

out vec3 fFragColor;
uniform sampler2D uTexture;

in vec2 vTexCoords;

void main() {
  fFragColor = texture(uTexture, vTexCoords).rgb;
};
