#version 330 core

out vec4 FragColor;
in vec3  NormVec;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float shininess;

void main()
{
	vec3 normal = normalize(NormVec);

	//Ambient Light
	float ambientstr = 0.1;
	vec3 ambient = ambientstr * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, normal);

	//Diffuse Light
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//Specular Light
	float specularstr = 0.5;
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularstr * lightColor * spec;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0);
}
