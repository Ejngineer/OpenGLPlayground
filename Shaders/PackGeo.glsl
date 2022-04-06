#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 Normal;
} gs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 projection;

void DrawLine(int index)
{
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].Normal, 0.0) * MAGNITUDE);
	EmitVertex();

	EndPrimitive();
}


void main()
{
	DrawLine(0);
	DrawLine(1);
	DrawLine(2);
}

