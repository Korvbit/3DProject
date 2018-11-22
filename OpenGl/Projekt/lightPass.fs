#version 440

out vec4 fragment_color;	
in vec2 texCoord0;

const int MAX_POINT_LIGHTS = 1;    

struct PointLight
{
	vec3 position;
	vec3 color;
};

uniform vec3 cameraPosLP;
uniform PointLight PointLights[MAX_POINT_LIGHTS];

// Gbuffer variables
uniform sampler2D gPosition;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;

void main()
{
	// Sample from the gBuffer
	vec3 pixelPos = texture(gPosition, texCoord0).xyz;
	vec3 materialColor = texture(gDiffuse, texCoord0).rgb;
	vec3 normal = texture(gNormal, texCoord0).xyz;

	// Attenuation
	float attenuation;
	float distancePixelToLight;

	// Ambient
	vec4 ambient = vec4(0.45f,0.45f,0.45f,1.0f) * vec4(materialColor.rgb, 1.0f);
	
	// Diffuse
	vec3 lightDir;
	vec4 diffuse;
	float alpha;
	
	// Specular
	vec3 vecToCam;
	vec4 reflection;
	vec4 specular;
	float shininess = 30;

	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		// Diffuse
		lightDir = normalize(PointLights[i].position.xyz - pixelPos.xyz);
		alpha = dot(normal.xyz,lightDir);
		diffuse += vec4(materialColor.rgb,1.0f) * vec4(PointLights[i].color.rgb, 1.0f) * max(alpha, 0);

		// Specular
		vecToCam = normalize(vec3(cameraPosLP.xyz - pixelPos.xyz));	
		// Source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml
		reflection = reflect(vec4(-lightDir.xyz, 0.0f), vec4(normal.xyz,1.0f));
		specular += vec4(materialColor.rgb,1.0f) * vec4(PointLights[i].color.rgb, 1.0f) * pow(max(dot(reflection.xyz, vecToCam.xyz),0), shininess);

		// attenuation
		distancePixelToLight = length(PointLights[i].position - pixelPos);
		attenuation = 1.0f / (1.0f + (0.1 * distancePixelToLight)+ (0.01 * pow(distancePixelToLight, 2)));
	}
	vec4 finalColor = ambient + attenuation*(diffuse + specular);
	finalColor = min(vec4(1.0f,1.0f,1.0f,1.0f), finalColor);

	fragment_color = vec4(finalColor.xyz, 1.0f);

	//fragment_color = vec4(normal, 1.0f);
	// Tester
	//fragment_color = vec4(finalColor.xyz, 1.0f);
	//fragment_color = vec4(materialColor.xyz, 1.0f);
	//fragment_color = vec4(1.0f,1.0f,0.0f,1.0f);
	//fragment_color = vec4(PointLights[1].color.rgb, 1.0f);
}