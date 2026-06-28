#version 430 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;

void main()
{
    vec4 texResult = texture(albedoTexture, TexCoord);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0) * texResult;
}