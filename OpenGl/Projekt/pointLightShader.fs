#version 440

in vec3 colorOut;

out vec4 fragment_color;

void main()
{
	fragment_color = vec4(colorOut.rgb, 1.0f);
}