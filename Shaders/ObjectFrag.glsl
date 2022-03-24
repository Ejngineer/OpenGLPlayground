#version 330 core

out vec4 FragColor;
in vec3  NormVec;
in vec3 FragPos;

in vec2 texCoords;

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define N_POINT_LIGHTS 4

uniform DirLight dirlight;
uniform PointLight plight[N_POINT_LIGHTS];

uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal,  vec3 viewDir);
vec3 CalcPointLight(PointLight plight, vec3 normal, vec3 fragpos, vec3 viewDir);

void main()
{
	//define output color value
	vec3 output = vec3(0.0);
	//add the directional light's contribution to the output

	vec3 viewDir = normalize(viewPos - FragPos);
	output += CalcDirLight(dirlight, NormVec, viewDir);

	for(int i = 0 ; i < 4; i++)
	{
		output += CalcPointLight(plight[i], NormVec, FragPos, viewDir);
	}

	FragColor = vec4(output, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight plight, vec3 normal, vec3 fragpos, vec3 viewDir)
{
	vec3 lightDir = normalize(plight.position - fragpos);
	float distance = length(plight.position - fragpos);

	float att = 1.0 / (plight.constant + plight.linear * distance + plight.quadratic * (distance * distance));
	
	vec3 ambient = plight.ambient * vec3(texture(material.diffuse, texCoords));

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = plight.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, normal),0.0), material.shininess);

	vec3 specular = plight.specular * spec * vec3(texture(material.specular, texCoords));

	ambient *= att;
	diffuse *= att;
	specular *= att;

	vec3 result = (ambient + diffuse + specular);

	return(ambient + diffuse + specular);
}
