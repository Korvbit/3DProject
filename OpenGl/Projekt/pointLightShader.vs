#version 440

in vec3 position;
in vec3 texCoord; // This is color in this case

out vec3 color;

uniform mat4 transformationMatrix;
uniform mat4 worldMatrix;

void main()
{
	gl_Position = transformationMatrix * vec4(position, 1.0f);
	color = texCoord;
}