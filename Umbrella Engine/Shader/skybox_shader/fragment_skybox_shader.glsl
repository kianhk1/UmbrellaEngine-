#version 430 core

in vec3 TexCoord;
uniform samplerCube skybox;

out vec4 FragColor;
uniform vec4 color;
uniform vec4 emissiveColor;

void main() {
    emissiveColor;
	FragColor = texture(skybox, TexCoord) * color;
}