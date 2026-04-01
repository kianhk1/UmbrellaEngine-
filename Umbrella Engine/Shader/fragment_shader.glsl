#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D texture1;

void main() {
	vec4 result = texture(texture1, TexCoord);
	if(result.a < 0.1)
        discard;
	FragColor = result;
}
