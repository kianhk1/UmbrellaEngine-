#version 430 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D albedoTexture;
uniform sampler2D metallicRoughnessTexture;
uniform sampler2D normalTexture;
uniform vec4 color;

void main() {
    vec4 texResult = texture(albedoTexture, TexCoord);
    vec4 texmetallicRoughness = texture(metallicRoughnessTexture, TexCoord);
    vec4 texnormal = texture(normalTexture, TexCoord);
    FragColor = texResult;
}