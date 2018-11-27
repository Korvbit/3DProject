#version 440

in vec3 color;

out vec4 fragment_color;

void main()
{
	fragment_color = vec4(color.rgb, 1.0f);
}