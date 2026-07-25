#version 430 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform vec4 color;

void main() {
    vec4 texResult = texture(albedoTexture, TexCoord);
    FragColor = color;
}