#version 450 core

#define MAX_SHADOW_MAPS 8

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aTangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform mat4 uLightSpaceMatrices[MAX_SHADOW_MAPS];

out vec3 vWorldPosition;
out vec3 vNormal;
out vec2 vUV;

out vec4 vLightSpacePositions[MAX_SHADOW_MAPS];

void main()
{
    vec4 worldPosition = uModel * vec4(aPosition, 1.0);

    vWorldPosition = worldPosition.xyz;

    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    vUV = aUV;

    for (int i = 0; i < MAX_SHADOW_MAPS; ++i)
    {
        vLightSpacePositions[i] = uLightSpaceMatrices[i] *  worldPosition;
    }

    gl_Position = uProjection * uView * worldPosition;
}