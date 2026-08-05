#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
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
uniform mat4 model;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

mat3 NormalMatrix(vec3 Tangent,vec3 Normal,mat4 model){
mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * Tangent);
    vec3 N = normalize(normalMatrix * Normal);
    vec3 B = cross(N, T);
    mat3 TBN;
    TBN = mat3(T, B, N);
    return TBN;
}


void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz; 

    TBN = NormalMatrix(aTangent,aNormal,model);
    
    FragPosLightSpace = lightSpaceMatrix * worldPos;

    ourColor = aColor;
    Normal = aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
