#version 430 core

layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (binding = 0) uniform Camradata
{
    mat4 projection;
    mat4 view;
    vec3 camposition;
};

out vec2 TexCoord;

uniform mat4 model;
void main()
{
    TexCoord = aTexCoord;
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
}