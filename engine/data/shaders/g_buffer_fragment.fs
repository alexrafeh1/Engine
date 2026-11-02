#version 460 core

layout(location=0) out vec4 gAlbedo;
layout(location=1) out vec4 gNormalRoughness;
layout(location=2) out vec2 gMaterial;

//Textures
uniform sampler2D uDiffuseTexture;
uniform sampler2D uNormalTexture;
uniform sampler2D uRoughnessTexture;
uniform sampler2D uMetallicTexture;
uniform sampler2D uAOTexture;

uniform int uHasAlbedo;
uniform int uHasNormal;
uniform int uHasRoughness;
uniform int uHasMetallic;
uniform int uHasAO;


//Color
uniform vec4 uColor;
uniform float uRoughness;
uniform float uMetallic;
uniform float uAO;

in vec3 Normal;
in vec2 UV;

void main(){

    gAlbedo = texture(uDiffuseTexture ,UV) * uColor;
    gNormalRoughness = vec4(normalize(Normal),uRoughness);
    gMaterial = vec2(uMetallic,uAO);

    if(uHasRoughness == 1){
        gNormalRoughness.a = texture(uRoughnessTexture,UV).r;
    }
    if(uHasMetallic == 1){
        gMaterial.r = texture(uMetallicTexture,UV).r;
    }
    if(uHasAO == 1){
        gMaterial.g = texture(uAOTexture,UV).r;
    }

}

