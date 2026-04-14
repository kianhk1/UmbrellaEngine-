#version 430 core

layout (binding = 0) uniform Camradata
{
    mat4 projection;
    mat4 view;
    vec3 camposition;
};
layout (binding = 1) uniform Light
{
    vec3 lightcolor;
    vec3 lightposition;
};

vec3 ambientcolor = vec3(0.3f);

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

vec3 CalcPointLight(vec3 normal, vec3 fragWorldPos, vec3 lightDir, vec3 viewDir, vec3 texColor) {

    vec3 normLightDir = normalize(lightDir);
    float diff = max(dot(normal, normLightDir), 0.0);

    vec3 ambient = ambientcolor * texColor; 
    vec3 diffuse = (diff * texColor); 
    
    // محاسبه نور بازتابی (specular) - اگر لازم باشد
    vec3 halfwayDir = normalize(normLightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), /*shininess*/ 64);
    vec3 specular = spec * texture(specularTexture, TexCoord).rgb;

    vec3 finalLight = ambient + (diffuse + specular ) * lightcolor; // ترکیب رنگ نور با رنگ سطح
    
    return finalLight;
}

void main() {

    vec4 texResult = texture(albedoTexture, TexCoord);
    if(texResult.a < 0.1)
        discard;

    vec3 normal = texture(normalTexture, TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 norm = normalize(TBN * normal);

    vec3 viewDir = normalize(camposition - FragPos); 
    vec3 lightDir = normalize(lightposition - FragPos);

    // محاسبه نور با استفاده از تابع CalcPointLight
    vec3 lighting = CalcPointLight(norm, FragPos, lightDir, viewDir, texResult.rgb);
    
    vec4 finalColor = vec4(lighting, texResult.a);

    FragColor = finalColor;
}
