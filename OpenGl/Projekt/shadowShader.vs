#version 440

in vec3 position;

uniform mat4 worldMatrix;

void main()
{
	// The vertex is transformed to worldspace and then sent to the GS
	gl_position = worldMatrix * vec4(position, 1.0f);
}