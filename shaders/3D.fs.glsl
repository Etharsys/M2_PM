#version 300 es

precision mediump float;

in vec3 vVertexPosition;
in vec3 vVertexNormal;
in vec2 vVertexTexture;

in vec3 vColor;

out vec3 fColor;

void main() {
    fColor = vColor;
};