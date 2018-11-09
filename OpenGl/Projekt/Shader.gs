#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 texCoord0[];

out vec2 texCoords;

uniform mat4 transformationMatrix; 
uniform mat4 WorldMatrix; 

void main()
{
	for(int i = 0; i < 3; i++)
	{
		gl_Position = transformationMatrix * gl_in[i].gl_Position;
		texCoords = texCoord0[i];
		EmitVertex();
	}

	EndPrimitive();
}