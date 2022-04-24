#version 330 core

out vec4 neptuneColor;

in vec2 TexCoords;

uniform sampler2D tex1;

void main()
{
	neptuneColor = texture(tex1, TexCoords); 
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
