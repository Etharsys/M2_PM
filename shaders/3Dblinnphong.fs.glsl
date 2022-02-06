#version 300 es

precision mediump float;

in vec3 vVertexPosition;
in vec3 vVertexNormal;
in vec2 vVertexTexture;

uniform vec3 uColor;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

out vec3 fColor;

float dot_pos(vec3 v1, vec3 v2){
    return max(0., dot(v1,v2));
}

vec3 blinnPhong(vec3 uKd){
    vec3 w0 = normalize(-vVertexPosition);
    vec3 wi = normalize(uLightDir_vs);
    vec3 halfvect = normalize((w0+wi)/2.);
    return uLightIntensity*(uKd*dot_pos(wi,normalize(vVertexNormal)));
}

void main() {
    fColor = blinnPhong(uColor);
};