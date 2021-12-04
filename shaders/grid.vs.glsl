#version 300 es

uniform vec2 uTranslation;
uniform vec3 uColor;

layout(location = 0) in vec2 aVertexPosition;

out vec3 vColor;

void main() {
    vColor = uColor;
    gl_Position = vec4(uTranslation + aVertexPosition,0, 1);
}