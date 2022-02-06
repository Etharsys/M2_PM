#version 300 es

uniform vec3 uTranslation;
uniform vec3 uColor;

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;

out vec3 vColor;

void main() {
    vColor = uColor;
    gl_Position = vec4(uTranslation + aVertexPosition,0, 1);
}