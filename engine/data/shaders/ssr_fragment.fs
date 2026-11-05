#version 460 core

// Textures
uniform sampler2D gLight;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gMaterial;
uniform sampler2D gDepth;

in vec2 UV;

// Camera
uniform mat4 UInverseProjection;
uniform mat4 uProjection;

out vec4 FragColor;

const int MAX_STEPS = 400;

const float STEP_SIZE = 0.05;
const float THICKNESS = 0.1;
const float BIAS = 0.01;

const float MAX_DISTANCE = 10.0;

vec3 ReconstructPosition(vec2 uv, float depth)
{
    float z = depth * 2.0 - 1.0;
    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = UInverseProjection * clipSpacePosition;
    return viewSpacePosition.xyz / viewSpacePosition.w;
}


void main()
{
    vec3 normal = normalize(texture(gNormal, UV).xyz);
    float depth = texture(gDepth, UV).r;

    
    vec3 position = ReconstructPosition(UV, depth);
    vec3 viewDir = normalize(position);

    vec3 reflectionDir = normalize(reflect(viewDir, normal));

    vec3 rayOrigin = position + normal * BIAS;
    vec3 rayDirection = reflectionDir;

    for(int i = 0;i < MAX_STEPS; i++)
    {
        float distance = float(i + 1) * STEP_SIZE;

        if (distance > MAX_DISTANCE)
            break;
        vec3 rayPos = rayOrigin + reflectionDir * distance;

        vec4 clipPos = uProjection * vec4(rayPos, 1.0);
        if (clipPos.w <= 0.0)
            break;

        clipPos.xyz /= clipPos.w;
        vec2 rayUV = clipPos.xy * 0.5 + 0.5;     
        if (rayUV.x < 0.0 || rayUV.x > 1.0 ||
            rayUV.y < 0.0 || rayUV.y > 1.0)
        {
            break;
        }

        float depth = texture(gDepth, rayUV).r;
        if (depth >= 1.0)
            continue;
            
        vec3 scenePos = ReconstructPosition(rayUV, depth);
        

        float rayDepth = -rayPos.z;
        float sceneDepth = -scenePos.z;

        float difference = rayDepth - sceneDepth;
        if(difference > 0.0 && difference < THICKNESS)
        {
            vec3 color = texture(gLight, rayUV).rgb;
            vec3 color2 = texture(gLight, UV).rgb;
            vec3 finalColor = mix(color2, color, 0.5);
           FragColor = vec4(finalColor, 1.0);
            return;
        }
    }

    vec3 color = texture(gLight, UV).rgb;
    FragColor = vec4(color, 1.0);
}