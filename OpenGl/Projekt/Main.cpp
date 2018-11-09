#include <iostream>
#include <glew\glew.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "ObjectHandler.h"
#include <ctime>
#include "timer.h"

// Finns en main funktion i GLEW, därmed måste vi undefinera den innan vi kan använda
#undef main
#define FRAMERATE 60

int SCREENWIDTH = 800;
int SCREENHEIGHT = 600;

int main()
{
	Display display(SCREENWIDTH, SCREENHEIGHT, "Executing Project");

	Shader shader;
	shader.CreateShader(".\\Shader.vs", GL_VERTEX_SHADER);
	shader.CreateShader(".\\Shader.gs", GL_GEOMETRY_SHADER);
	shader.CreateShader(".\\Shader.fs", GL_FRAGMENT_SHADER);

	shader.initateShaders();
	
	Camera camera(glm::vec3(0, 0, -3), 70.0f,(float)SCREENWIDTH / (float)SCREENHEIGHT, 0.01f, 1000.0f);

	Vertex vertices[] = {	Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0,0.0)),
							Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5,1.0)),
							Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0,0.0))
	};


	// Ett Object med mesh, transform och texture
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	Transform transform;
	Texture texture("Textures/basicSnow.jpg");

	ObjectHandler OH = ObjectHandler();
	Object aObject = OH.CreateObject(&mesh, transform, &texture);

	float counter = 0.0f;

	// Initiate clock
	timer_lib_initialize();
	tick_t deltaTime, currTime, lastTime, startTime;
	startTime = timer_current();
	lastTime = timer_current();

	while (!display.IsWindowClosed())
	{
		// Ta reda på tiden
		currTime = timer_current();
		deltaTime = timer_elapsed(lastTime)*FRAMERATE;

		// Ser till att inte överskrida FRAMERATE
		if (deltaTime >= 1.0f)
		{

			glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);			//EVENTUELLT gör detta till egen funktion i Display

			float cosCounter = cosf(counter);	// Test Grej

			aObject.GetPos().x = sinf(counter);	// Test Grej
			aObject.GetRot().z = counter;	// Test Grej
			//transform.SetScale(glm::vec3(cosCounter, cosCounter, cosCounter)); // Test Grej

			texture.Bind(0); // Kanske denna med???

			// ------------ När vi har flera så skall detta vara innom en loop ------------
			shader.Bind();
			shader.Update(aObject.GetTransform(), camera);
			aObject.Draw();
			// ----------------------------------------------------------------------------

			display.SwapBuffers();
			counter += 0.01f;				// Test Grej

			lastTime = currTime;
			deltaTime -= 1;
		}
		else {
			// Not time to do anything yet
		}
	}

	return 0;
}