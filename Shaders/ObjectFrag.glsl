#version 330 core

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light 
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in vec3  NormVec;
in vec3 FragPos;

in vec2 texCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	//Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	//Diffuse
	vec3 norm = normalize(NormVec);
	vec3 lightdir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightdir), 0.0);

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	//Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightdir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}