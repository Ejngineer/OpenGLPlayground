#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos; 
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuse_texture1;
uniform sampler2D specular_texture1;

void main()
{
	gPosition = FragPos;
	gNormal = normalize(Normal);
	gAlbedoSpec.rgb = texture(diffuse_texture1, TexCoords).rgb;
	gAlbedoSpec.a = texture(specular_texture1, TexCoords).r;

}
