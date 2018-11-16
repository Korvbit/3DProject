#version 440

in vec2 texCoords;
in vec4 normalWorld1;
in vec4 posWorld1;

out vec3 WorldPosOut;
out vec3 TextureRGBOut;
out vec3 WorldNormalOut;

uniform sampler2D textureUnit;
void main()
{	
	// Deferred rendering Geometry pass
	WorldPosOut = posWorld1.xyz;
	TextureRGBOut = texture2D(textureUnit,texCoords).xyz;
	WorldNormalOut = normalize(normalWorld1).xyz;
}