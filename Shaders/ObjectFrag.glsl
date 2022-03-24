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
	vec3 direction;
	float cutoff;
	float outercutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
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
	//fade
	float theta;
	float epsilon;
	float intensity;
	//Attenuation
	float distance;
	float att;
	//Ambient
	vec3 ambient;
	//Diffuse
	vec3 norm;
	vec3 lightdir;
	float diff;
	vec3 diffuse;
	//Specular
	vec3 viewDir;
	vec3 reflectDir;
	float spec;
	vec3 specular;

	//Calculations
	ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	
	lightdir = normalize(light.position - FragPos);
	  
	distance = length(light.position - FragPos);
	att = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * pow(distance, 2)));

	norm = normalize(NormVec);
	diff = max(dot(norm, lightdir), 0.0); 
	diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

	viewDir = normalize(viewPos - FragPos);
	reflectDir = reflect(-lightdir, norm);

	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	theta = dot(lightdir, normalize(-light.direction));
	epsilon = light.cutoff - light.outercutoff;
	intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	vec3 result = (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0f);	
}