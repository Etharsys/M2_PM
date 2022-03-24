#version 300 es

precision mediump float;

in vec3 vVertexPosition;
in vec3 vVertexNormal;
in vec2 vVertexTexture;

uniform vec4 uColor;

out vec4 fColor;

void main() {
    fColor = uColor;
};