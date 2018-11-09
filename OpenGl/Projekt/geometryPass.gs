#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 texCoord0[];

out vec2 texCoords;
out vec4 normalWorld1;
out vec4 posWorld1;

uniform mat4 transformationMatrix; 
uniform mat4 WorldMatrix; 

uniform vec3 cameraPosGP;

// functions
vec4 createNormalWorld();

void main()
{	
	vec4 normalWorld = createNormalWorld();
	
	vec4 posWorld[3];
	for(int i = 0; i < 3; i++)
	{
		posWorld[i] = WorldMatrix * gl_in[i].gl_Position;
	}

	// Backface culling using geometry shader
	float alpha = dot(normalWorld.xyz,cameraPosGP.xyz - posWorld[1].xyz);
	if(alpha > 0)
	{
		for(int i = 0; i < 3; i++)
		{
			gl_Position = transformationMatrix * gl_in[i].gl_Position;
			texCoords = texCoord0[i];
			normalWorld1 = normalWorld;
			posWorld1 = posWorld[i];
			EmitVertex();
		}
		EndPrimitive();
	}	
}

vec4 createNormalWorld()
{
	vec4 vectorUsedForNormal1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 vectorUsedForNormal2 = gl_in[2].gl_Position - gl_in[0].gl_Position;  

	vec4 normal = vec4(normalize(cross(vectorUsedForNormal1.xyz, vectorUsedForNormal2.xyz)), 0.0f);
	vec4 normalWorld = normalize(WorldMatrix * normal);

	return normalWorld;	
}