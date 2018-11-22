#version 440

in vec2 texCoords;
in vec4 posWorld1;
in mat4 TBN;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;

uniform sampler2D texture;
uniform sampler2D normalMap;

void main()
{	
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld1.xyz;
	TextureRGBOut = texture2D(texture,texCoords).xyz;
	// Sample the normalMap and adjust the values to the range [-1,1], (range is originally [0,1])
	WorldNormalOut = normalize(texture2D(normalMap, texCoords).rgb * 2 - 1.0f).xyz;
	// Transform the normal from tangent space to world space
	WorldNormalOut = normalize(vec3(TBN * vec4(WorldNormalOut, 0.0f)));
}