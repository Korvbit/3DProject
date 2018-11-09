#version 440

uniform sampler2D diffuse;

in vec2 texCoords;

out vec4 fragment_color;

void main()
{
	// Standard utvärde som vi kanske kan använda sen
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);

	fragment_color = texture2D(diffuse,texCoords);
}