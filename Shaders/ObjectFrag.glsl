#version 330 core

out vec4 FragColor;
in vec3  NormVec;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(NormVec);
	vec3 lightdir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightdir), 0.0);
	vec3 Diffuse = diff * lightColor;

	float ambientStr = 0.1;
	vec3 Ambient = ambientStr * lightColor;

	float specularStrength = 0.5;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightdir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 Specular = specularStrength * spec * lightColor;

	vec3 result = (Ambient + Diffuse + Specular) * objectColor;

	FragColor = vec4(result, 1.0f);
}