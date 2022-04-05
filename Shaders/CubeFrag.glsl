#version 330 core

out vec4 FragColor;

in vec3 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform vec3 camPos;
uniform samplerCube cubeMap;

void main()
{
	vec3 I = normalize(Position - camPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}