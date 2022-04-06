#version 330 core

out vec4 FragColor;

in vec2 Tex;

uniform sampler2D Texture_diffuse1;

void main()
{
	FragColor = texture(Texture_diffuse1, Tex);
}