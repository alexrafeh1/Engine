#version 460 core

//textures
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gMaterial;
uniform sampler2D gDepth;


uniform int uFirstPass;

in vec2 UV;

//lights
uniform vec3 light_dirr;
uniform vec3 light_pos;
uniform vec3 light_color;


//Cam
uniform vec3 uCamPos;
uniform mat4 UInverseProjection;
uniform mat4 uInverseView;

out vec4 FragColor;

const float PI = 3.14159265359;


vec3 GetWorldPosition(vec2 uv, float depth, mat4 invProjection, mat4 invView) {
    vec2 ndc = uv * 2.0 - 1.0;
    vec4 clip = vec4(ndc, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = invProjection * clip;
    viewPos /= viewPos.w;
    vec3 worldPos = (invView * viewPos).xyz;
    return worldPos;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main(){
    vec4 normalRoughness = texture(gNormal, UV);
    vec3 albedo = texture(gDiffuse, UV).rgb;
    vec3 worldPos = GetWorldPosition(UV, texture(gDepth, UV).r, UInverseProjection, uInverseView);
    
    vec3 N = normalize(normalRoughness.rgb);
    vec3 L = normalize(-light_dirr);
    float NdotL = max(dot(N, L), 0.0);
    vec3 V = normalize(uCamPos - worldPos);
    vec3 H = normalize(V + L);
    float roughness = clamp(normalRoughness.a, 0.045, 1.0);
    float metallic = texture(gMaterial, UV).r;
    float ao = texture(gMaterial, UV).g;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 Lo = (kD * albedo / PI + specular) * light_color * NdotL;

    vec3 color;
    if(uFirstPass == 1){
      vec3 ambient = vec3(0.03) * albedo * ao;
      color = ambient + Lo;
      color = color / (color + vec3(1.0));
      color = pow(color, vec3(1.0/2.2));
    }

    FragColor = vec4(color, 1.0);
}
