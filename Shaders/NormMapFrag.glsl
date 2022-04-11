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



void main()
{
	float ambientstr = 0.3;	
	vec3 ambient = ambientstr * vec3(texture(walltexture, fs_in.TexCoords));

	//vec3 normal = normalize(fs_in.Normal);
	vec3 normal = texture(walltexturenorm, fs_in.TexCoords).rgb;
	normal = normal * 2.0 - 1.0;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.FragPos);
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.FragPos);

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * vec3(texture(walltexture, fs_in.TexCoords));

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 16.0);

	vec3 specular = spec * vec3(texture(walltexture, fs_in.TexCoords));

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}
