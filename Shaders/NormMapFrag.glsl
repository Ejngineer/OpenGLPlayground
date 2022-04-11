#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D walltexture;
uniform sampler2D walltexturenorm;
uniform sampler2D walltexturedepth;

uniform float heightscale;

vec2 ParallaxMapping(vec2 TexCoords, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);

	if(texCoords.x > 1.0 ||
		texCoords.x < 0.0 ||
		texCoords.y > 1.0 ||
		texCoords.y < 0.0)
		{
			discard;
		}

	float ambientstr = 0.3;	
	vec3 ambient = ambientstr * vec3(texture(walltexture, texCoords));

	//vec3 normal = normalize(fs_in.Normal);
	vec3 normal = texture(walltexturenorm, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.FragPos);

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * texture(walltexture, texCoords).rgb;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);

	vec3 specular = spec * vec3(0.2);//texture(walltexture, fs_in.TexCoords).rgb;

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}

vec2 ParallaxMapping(vec2 TexCoords, vec3 viewDir)
{
	const float minLayers = 8.0;
	const float maxLayers = 32.0;

	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0,0.0,1.0), viewDir),0.0));
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;

	float height = texture(walltexturedepth, TexCoords).r;
	vec2 p = viewDir.xy / viewDir.z * (height * heightscale);
	vec2 deltaTexCoords = p / numLayers;

	vec2 currentTexCoords = TexCoords;
	float currentDepthMapValue = texture(walltexturedepth, currentTexCoords).r;

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(walltexturedepth, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(walltexturedepth, prevTexCoords).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;
}
