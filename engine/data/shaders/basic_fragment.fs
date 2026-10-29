#version 450 core

struct GPULight
{
    vec4 position;
    vec4 color;
    vec4 direction;
    vec4 parameters;
};

layout(std430, binding = 0) readonly buffer LightBuffer
{
    GPULight lights[];
};

uniform int uLightCount;
uniform sampler2D uTexture;
uniform vec3 uCameraPosition;
uniform vec4 uColor;

uniform vec3 uPlanetCenter;
uniform float uPlanetRadius;

#define MAX_SHADOW_MAPS 8

uniform sampler2D uShadowMaps[MAX_SHADOW_MAPS];
uniform int uShadowMapCount;

in vec3 vWorldPosition;
in vec3 vNormal;
in vec2 vUV;
in vec4 vLightSpacePositions[MAX_SHADOW_MAPS];

out vec4 FragColor;


// --------------------------------------------------
// Sombras
// Devuelve 0 si no hay sombra y 1 si está en sombra.
// --------------------------------------------------

float CalculateShadow(
    sampler2D shadowMap,
    vec4 lightSpacePosition,
    vec3 normal,
    vec3 lightDirection)
{
    if (lightSpacePosition.w <= 0.0)
        return 0.0;

    vec3 projCoords =
        lightSpacePosition.xyz / lightSpacePosition.w;

    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 0.0;
    }

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(
        0.0010 * (1.0 - max(dot(normal, lightDirection), 0.0)),
        0.0001
    );

    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}


// --------------------------------------------------
// Material procedural
// --------------------------------------------------

vec3 CalculateTerrainColor(
    vec3 position,
    vec3 normal,
    vec3 radialUp)
{
    float radiusAtPoint = length(position - uPlanetCenter);

    // Altura radial respecto al radio base.
    float height = radiusAtPoint - uPlanetRadius;

    // 0 = superficie horizontal respecto al planeta.
    // 1 = pared aproximadamente vertical.
    float slope = 1.0 - clamp(
        dot(normal, radialUp),
        0.0,
        1.0
    );

    // Paleta alienígena.
    vec3 sand  = vec3(0.64, 0.48, 0.31);
    vec3 grass = vec3(0.24, 0.48, 0.28);
    vec3 rock  = vec3(0.36, 0.34, 0.39);
    vec3 snow  = vec3(0.78, 0.84, 0.86);
    vec3 dirt  = vec3(0.25, 0.16, 0.13);

    // Tierras bajas y regiones de altura moderada.
    vec3 color = mix(
        sand,
        grass,
        smoothstep(-1.0, 8.0, height)
    );

    // Cumbres altas.
    color = mix(
        color,
        snow,
        smoothstep(35.0, 65.0, height)
    );

    // La roca domina en pendientes pronunciadas.
    color = mix(
        color,
        rock,
        smoothstep(0.25, 0.65, slope)
    );

    // En excavaciones, las paredes profundas
    // reciben un material distinto.
    float depth = max(-height, 0.0);

    color = mix(
        color,
        dirt,
        smoothstep(1.0, 4.0, depth)
    );

    color = mix(
        color,
        rock * 0.65,
        smoothstep(8.0, 20.0, depth)
    );

    return color;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPosition - vWorldPosition);

    // Dirección radial del planeta en coordenadas mundo.
    vec3 radial = vWorldPosition - uPlanetCenter;
    float radialLength = length(radial);

    vec3 radialUp = radialLength > 1e-6
        ? radial / radialLength
        : normal;

    vec3 terrainColor = CalculateTerrainColor(
        vWorldPosition,
        normal,
        radialUp
    );

    // Textura opcional usada como tinte del material.
    vec3 textureTint = texture(uTexture, vUV).rgb;
    vec3 albedo = terrainColor * uColor.rgb;

    // Iluminación hemisférica: evita que las caras oscuras
    // desaparezcan completamente.
    float hemi = 0.5 + 0.5 * dot(normal, radialUp);

    vec3 ambient = mix(
        vec3(0.045, 0.035, 0.035),
        vec3(0.20, 0.25, 0.32),
        hemi
    );

    vec3 result = albedo * ambient;

    int lightCount = min(uLightCount, lights.length());
    int shadowCount = clamp(uShadowMapCount, 0, MAX_SHADOW_MAPS);

    for (int i = 0; i < lightCount; ++i)
    {
        GPULight light = lights[i];

        // Este shader procesa aquí únicamente luces direccionales.
        if (light.direction.w != 0.0)
            continue;

        vec3 lightDir = normalize(-light.direction.xyz);

        float NdotL = max(dot(normal, lightDir), 0.0);

        if (NdotL <= 0.0)
            continue;

        float shadow = 0.0;

        if (i < shadowCount)
        {
            shadow = CalculateShadow(
                uShadowMaps[i],
                vLightSpacePositions[i],
                normal,
                lightDir
            );
        }

        // Luz difusa directa.
        vec3 diffuse = albedo * light.color.rgb * NdotL;

        // Brillo especular.
        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(
            max(dot(viewDir, reflectDir), 0.0),
            32.0
        );

        vec3 specular = light.color.rgb * spec * 0.2;

        // La sombra afecta a la luz directa, no al ambiente.
        result += (1.0 - shadow) * (diffuse + specular);
    }

    FragColor = vec4(result, uColor.a);
}