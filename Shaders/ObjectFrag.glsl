#version 330 core

out vec4 FragColor;

in vec3 NormVec;
in vec3 FragPos;
in vec2 texCoords;

struct PointLight
{
	vec3 Color;
	vec3 position;
};
#define N_POINT_LIGHTS 4

uniform PointLight plight[N_POINT_LIGHTS];

uniform vec3 viewPos;
uniform sampler2D diffuseTexture;

vec3 CalcPointLight(PointLight plight, vec3 normal, vec3 fragpos, vec3 viewDir);

void main()
{
	//define output color value
	vec3 output = vec3(0.0);
	//add the directional light's contribution to the output

	vec3 viewDir = normalize(viewPos - FragPos);
	//output += CalcDirLight(dirlight, NormVec, viewDir);

	vec3 normal = normalize(NormVec);

	vec3 ambient = 0.0 * vec3(texture(diffuseTexture, texCoords));

	for(int i = 0 ; i < 4; i++)
	{
		output += (ambient + CalcPointLight(plight[i], normal, FragPos, viewDir));
	}

	FragColor = vec4(output, 1.0);
}

vec3 CalcPointLight(PointLight plight, vec3 normal, vec3 fragpos, vec3 viewDir)
{
	vec3 lightDir = normalize(plight.position - fragpos);
	float distance = length(fragpos - plight.position);

	//float att = 1.0 / (plight.constant + plight.linear * distance + plight.quadratic * (distance * distance));
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = plight.Color * diff * vec3(texture(diffuseTexture, texCoords));

	//vec3 reflectDir = reflect(-lightDir, normal);
	//vec3 halfwayDir = normalize(viewDir + lightDir);
	//float spec = pow(max(dot(halfwayDir, normal),0.0), 32.0);

	//vec3 specular = spec * plight.Color * vec3(texture(diffuseTexture, texCoords));

	//ambient *= att;
	diffuse *= 1.0 / (distance * distance);
	//specular *= att;

	vec3 result = diffuse;

	return result;
}
