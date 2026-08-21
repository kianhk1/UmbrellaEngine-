#version 430 core

layout (binding = 0) uniform Camradata {
    mat4 projection;
    mat4 view;
    vec3 camposition;
};
layout (binding = 1) uniform Light {
    vec3 lightcolor;
    vec3 lightposition;
    mat4 lightSpaceMatrix;
    int type;
};

vec3 ambientcolor = vec3(0.1f);

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in mat3 TBN;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D albedoTexture;
uniform sampler2D metallicRoughnessTexture;
uniform sampler2D normalTexture;
uniform sampler2D shadowMap;
const float PI = 3.14159265359;

vec3 fresnelSchlick(float NdotH, vec3 F0) {
    return F0 + (1-F0)*pow(1.0 - NdotH, 5.0);
}

float DistributionGGX(float NdotH,float roughness) {
float a = roughness * roughness;
float a2 = a * a;
float NdotH2= NdotH * NdotH;
float num = a2;
float denom=(NdotH2 * (a2 - 1.0) + 1.0);
denom = PI * denom * denom;
return num / denom;
}

float GeometrySchlickGGX(float NdotV,float roughness) {
float r = (roughness + 1.0);
float k = (r * r) / 8.0;
float num = NdotV;
float denom = NdotV * (1.0 - k) + k;
return num / denom;
}

float GeometrySmith(float NdotV,float NdotL,float roughness) {
float ggx2 = GeometrySchlickGGX(NdotV, roughness);
float ggx1 = GeometrySchlickGGX(NdotL, roughness);
return ggx1 * ggx2;
}

vec3 CalcPointLight(vec3 normal, vec3 fragWorldPos, vec3 lightDir, vec3 viewDir, vec3 texColor,vec3 mr,float shadow) {
    float roughness = mr.g;
    float metallic = mr.b;

    vec3 normLightDir = normalize(lightDir);
    float diff = max(dot(normal, normLightDir), 0.0);

    vec3 ambient = ambientcolor * texColor; 
    vec3 diffuse = (diff * texColor * (1.0-metallic)); 
    
    // محاسبه نور بازتابی (specular) - اگر لازم باشد
    vec3 halfwayDir = normalize(normLightDir + viewDir);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, texColor, metallic);
    float ndoth = max(dot(normal, halfwayDir), 0.0);
    float ndotv = max(dot(normal, viewDir), 0.0);
    float ndotl = max(dot(normal, lightDir), 0.0);
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float D = DistributionGGX(ndoth, roughness);
    float G = GeometrySmith(ndotv,ndotl, roughness);

    vec3 specular = (F * D * G) / max((4.0 * ndotv * ndotl), 0.001);

    vec3 finalLight = ambient + (diffuse + specular ) * lightcolor * ndotl * shadow; // ترکیب رنگ نور با رنگ سطح
    
    return finalLight;
}

void main() {
    color;
    vec4 texResult = texture(albedoTexture, TexCoord);
    if(texResult.a < 0.1)
        discard;

    vec3 normal = texture(normalTexture, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 norm = normalize(TBN * normal);

    vec3 mr =
        texture(metallicRoughnessTexture,
                TexCoord).rgb;

    vec3 viewDir = normalize(camposition - FragPos); 
    vec3 lightDir;
    if(type == 1)
        lightDir = normalize(-lightposition);
    else
       lightDir = normalize(lightposition - FragPos);

    vec3 projCoords =
        FragPosLightSpace.xyz / FragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth =
        texture(shadowMap, projCoords.xy).r;

    float currentDepth =
        projCoords.z;
    float bias = mix(0.0005, 0.0005, max(dot(normal, lightDir), 0.05));
    float shadow = 0.0;
       //currentDepth > closestDepth + bias ? 1.0 : 0.0;

       vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x =-1; x <= 1; ++x){
        for(int y =-1; y <= 1; ++y){
           float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // محاسبه نور با استفاده از تابع CalcPointLight
    vec3 lighting = CalcPointLight(norm, FragPos, lightDir, viewDir, texResult.rgb,mr,(1.0 - shadow));
    vec4 finalColor = vec4(lighting, texResult.a);

    FragColor = finalColor;
}
