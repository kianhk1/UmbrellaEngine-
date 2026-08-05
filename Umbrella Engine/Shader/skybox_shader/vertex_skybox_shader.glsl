#version 430 core

layout(location = 0) in vec3 aPos;
layout (binding = 0) uniform Camradata {
    mat4 projection;
    mat4 view;
    vec3 camposition;
};

uniform mat4 model;

out vec3 TexCoord;

void main()
{
    model;
    gl_Position = (projection * mat4(mat3(view)) * vec4(aPos,1.0)).xyww;
    TexCoord = aPos;
}