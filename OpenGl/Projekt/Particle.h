/*#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm\glm.hpp>
#include <glew\glew.h>

#define maxParticles 10000

struct ParticleStruct
{
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
};

class Particle
{
public:
	Particle();
	~Particle();
private:
	ParticleStruct particle[maxParticles]; 
};

#endif*/