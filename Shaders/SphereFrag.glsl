#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D tex1;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 color = vec3(0.5, 0.0, 0.0);
	float ambientstr = 0.1;
	vec3 ambient = ambientstr * color;

	vec3 normal = normalize(Normal);

	FragColor = vec4(color, 1.0);
}
