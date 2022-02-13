#version 300 es
precision mediump float;

out vec3 fFragColor;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

in vec2 vTexCoords;

void main() {
  fFragColor = texture(uEarthTexture, vTexCoords).rgb + texture(uCloudTexture, vTexCoords).rgb;
};
