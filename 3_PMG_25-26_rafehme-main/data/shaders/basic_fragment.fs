#version 450 core

struct GPULight
{
    vec4 position;
    vec4 color;
    vec4 direction;
    vec4 parameters;
};

layout(std430, binding = 0)
readonly buffer LightBuffer
{
    GPULight lights[];
};

uniform int uLightCount;
uniform sampler2D uTexture;
uniform vec3 uCameraPosition;

in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vUV;


out vec4 FragColor;


#define MAX_SHADOW_MAPS 8

in vec4 vLightSpacePositions[MAX_SHADOW_MAPS];
uniform sampler2D uShadowMaps[MAX_SHADOW_MAPS];
uniform int uShadowMapCount;




vec3 CalculateDirectionalLight(GPULight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir = normalize(vec3(-light.direction));

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color.rgb;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * light.color.rgb;

    // Ambient
    vec3 ambient = 0.05 * light.color.rgb;

    return ambient + diffuse + specular;
}


float CalculateShadow(sampler2D shadowMap,vec4 lightSpacePosition,vec3 normal,vec3 lightDirection){
    
    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;

    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 ||
        projCoords.x > 1.0 ||
        projCoords.y < 0.0 ||
        projCoords.y > 1.0 ||
        projCoords.z > 1.0){
        return 0.0;
    }

    float closestDepth = texture(shadowMap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float bias = max(0.0005 * (1.0 - dot(normal, lightDirection)),0.00005);

    if (currentDepth - bias > closestDepth)
        return 1.0;

    return 0.0;
}

void main()
{
    vec3 albedo = texture(uTexture, vUV).rgb;
    vec3 normal = normalize(vNormal);

    vec3 viewDir = normalize(uCameraPosition - vWorldPosition);

    vec3 result = vec3(0.0);


    for (int i = 0; i < uLightCount; ++i)
    {
        GPULight light = lights[i];
        // Check if the light is directional

        float shadow = CalculateShadow(uShadowMaps[i], vLightSpacePositions[i], normal, normalize(vec3(-light.direction)));

        if(light.direction.w == 0.0){
            result += (1.0 - shadow) * CalculateDirectionalLight(light, normal, viewDir) * albedo;
        }
    }

    FragColor = vec4(result, 1.0);
}