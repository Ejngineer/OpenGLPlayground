#version 330 core

out vec4 FragColor;
in vec3  NormVec;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	vec3 norm = normalize(NormVec);
	vec3 lightdir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightdir), 0.0);
	vec3 Diffuse = diff * lightColor;

	float ambientStr = 0.1;
	vec3 Ambient = ambientStr * lightColor;

	vec3 result = (Ambient + Diffuse) * objectColor;

	FragColor = vec4(result, 1.0f);
}