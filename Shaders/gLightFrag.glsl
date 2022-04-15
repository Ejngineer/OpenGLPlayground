#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Pos_tex;
uniform sampler2D Norm_tex;
uniform sampler2D AlbSpec_tex;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float radius;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
	vec3 FragPos = texture(Pos_tex, TexCoords).rgb;
	vec3 Normal = texture(Norm_tex, TexCoords).rgb;
	vec3 diffuse = texture(AlbSpec_tex, TexCoords).rgb;
	float specular = texture(AlbSpec_tex, TexCoords).a;

    //light attenuation
    

	vec3 lighting  = diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        float distance = length(lights[i].Position - FragPos);
        if(distance < lights[i].radius)
        {
            // diffuse
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * diffuse * lights[i].Color;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].Color * spec * specular;
            // attenuation
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }
    FragColor = vec4(lighting, 1.0);
}
