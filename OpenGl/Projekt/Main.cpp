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
#include "GBuffer.h"
#include "PointLight.h"
#include "Particle.h"
#include <stdio.h>
// Finns en main funktion i GLEW, därmed måste vi undefinera den innan vi kan använda våran main
#undef main

#define PI 3.1415926535

enum objectIndices
{
	cube1,
	cube2,
	sword,
	ground,
	moon,
	nrOfIndices
};

int SCREENWIDTH = 800;
int SCREENHEIGHT = 600;

// Deferred Rendering Functions
void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, Texture* snowTexture, Texture* swordTexture);
void DRLightPass(GBuffer *gBuffer, Mesh *fullScreenQuad, GLuint *program, Shader *geometryPass);
void particlePass(Particle * particle, Camera * camera, Shader * particleShader, float deltaTime);

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera);
void prepareTexture(GLuint textureLoc, GLuint normalMapLoc);

void setStartPositions(ObjectHandler *OH);

void keyboardControls(Display *display, Camera *camera);
void mouseControls(Display *display, Camera *camera);

int main()
{
	Display display(SCREENWIDTH, SCREENHEIGHT);

	Shader geometryPass;
	geometryPass.CreateShader(".\\geometryPass.vs", GL_VERTEX_SHADER);
	geometryPass.CreateShader(".\\geometryPass.gs", GL_GEOMETRY_SHADER);
	geometryPass.CreateShader(".\\geometryPass.fs", GL_FRAGMENT_SHADER);
	
	Shader lightPass;
	lightPass.CreateShader(".\\lightPass.vs", GL_VERTEX_SHADER);
	lightPass.CreateShader(".\\lightPass.fs", GL_FRAGMENT_SHADER);

	Shader particleShader;
	particleShader.CreateShader(".\\particleShader.vs", GL_VERTEX_SHADER);
	particleShader.CreateShader(".\\particleShader.fs", GL_FRAGMENT_SHADER);

	Shader pointLightPass;
	pointLightPass.CreateShader(".\\pointLightShader.vs", GL_VERTEX_SHADER);
	pointLightPass.CreateShader(".\\pointLightShader.fs", GL_FRAGMENT_SHADER);

	geometryPass.initiateShaders();
	lightPass.initiateShaders();
	particleShader.initiateShaders();
	pointLightPass.initiateShaders();
	
	Camera camera(glm::vec3(-15, 25, -53), 70.0f,(float)SCREENWIDTH / (float)SCREENHEIGHT, 0.01f, 1000.0f);
	



	//=========================== Creating Objects ====================================//

	Transform transform;
	Texture swordTexture("Textures/swordTexture.jpg", "NormalMaps/sword_normal.png");
	Texture brickTexture("Textures/brickwall.jpg", "NormalMaps/brickwall_normal.jpg");
	Texture snowTexture("Textures/basicSnow.jpg", "NormalMaps/flat_normal.jpg");
	Texture moonTexture("Textures/moon.png", "NormalMaps/flat_normal.jpg");

	ObjectHandler OH = ObjectHandler();

	Mesh cubeMesh;
	Mesh swordMesh;
	Mesh groundMesh;
	Mesh moonMesh;

	int cubes[2];
	for (int i = 0; i < 2; i++)
	{
		cubes[i] = OH.CreateObject("ObjectFiles/cube.obj", &cubeMesh, transform, &brickTexture);
	}
	int sword = OH.CreateObject("ObjectFiles/srd.obj", &swordMesh, transform, &swordTexture);
	int ground = OH.CreateObject("ObjectFiles/SnowTerrain.obj", &groundMesh, transform, &snowTexture);
	int moon = OH.CreateObject("ObjectFiles/moon.obj", &moonMesh, transform, &moonTexture);

	setStartPositions(&OH);
	//=================================================================================//

	GBuffer gBuffer;
	gBuffer.Init(SCREENWIDTH, SCREENHEIGHT);

	// https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/
	Vertex fullScreenVerticesTriangle[] =
	{ 
	Vertex(glm::vec3(-1, 3, 0), glm::vec2(0.0,2.0)),
	Vertex(glm::vec3(-1, -1, 0), glm::vec2(0.0,0.0)),
	Vertex(glm::vec3(3, -1, 0), glm::vec2(2.0,0.0)),
	};

	Mesh fullScreenTriangle(fullScreenVerticesTriangle, (sizeof(fullScreenVerticesTriangle) / sizeof(fullScreenVerticesTriangle[0])));

	double counter = 0.0;

	// Initiate timer
	double currentTime = glfwGetTime();
	double lastTime = 0;
	double deltaTime = 0;


	// Create Lights
	PointLightHandler lights;
	lights.createLight(glm::vec3(10.0f, 7.0f, -3.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	lights.createLight(glm::vec3(-7.0f, 7.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.createLight(glm::vec3(0.0f, 7.0f, -20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	lights.createLight(glm::vec3(4.0f, 7.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lights.createLight(glm::vec3(-4.0f, 7.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.5f));
	lights.createLight(glm::vec3(0.0f, 7.0f, 6.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	
	lights.initiateLights(lightPass.getProgram());

	Particle particle;
	
	// Tell the shaders the name of the camera (GP = GeometeryPass, LP = LightPass)
	GLuint cameraLocationGP = glGetUniformLocation(*geometryPass.getProgram(), "cameraPosGP");
	GLuint cameraLocationLP = glGetUniformLocation(*lightPass.getProgram(), "cameraPosLP");

	GLint texLoc;
	GLint normalTexLoc;

	texLoc = glGetUniformLocation(*geometryPass.getProgram(), "texture");
	normalTexLoc = glGetUniformLocation(*geometryPass.getProgram(), "normalMap");

	while(!display.IsWindowClosed())
	{
		deltaTime = currentTime - lastTime;
		lastTime = glfwGetTime();
		
		geometryPass.Bind();

		sendCameraLocationToGPU(cameraLocationGP, &camera);
		prepareTexture(texLoc, normalTexLoc);

		// Here all the objets gets transformed, and then sent to the GPU with a draw call
		DRGeometryPass(&gBuffer, counter, &geometryPass, &camera, &OH, &snowTexture, &swordTexture);
		geometryPass.unBind();

		lightPass.Bind();

		lights.sendToShader();
		sendCameraLocationToGPU(cameraLocationLP, &camera);
		
		// Here the fullscreenTriangel is drawn, and lights are sent to the GPU
		DRLightPass(&gBuffer, &fullScreenTriangle, lightPass.getProgram(), &lightPass);
		lightPass.unBind();

		// Draw particles
		particlePass(&particle, &camera, &particleShader, deltaTime);

		// Draw lightbölbz
		pointLightPass.Bind();
		lights.Draw();
		pointLightPass.unBind();

		// Check for mouse/keyboard inputs and handle the camera movement
		mouseControls(&display, &camera);
		keyboardControls(&display, &camera);

		camera.updateViewMatrix();

		//std::cout << "x: " << camera.getCameraPosition().x << " y: " << camera.getCameraPosition().y << " z: " << camera.getCameraPosition().z << std::endl;

		display.SwapBuffers(SCREENWIDTH, SCREENHEIGHT);

		counter += deltaTime * 0.5;
		currentTime = glfwGetTime();
	}
	return 0;
}

void DRGeometryPass(GBuffer *gBuffer, double counter, Shader *geometryPass, Camera *camera, ObjectHandler *OH, Texture *snowTexture, Texture *swordTexture)
{
	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//EVENTUELLT gör detta till egen funktion i Display
	
	glEnable(GL_DEPTH_TEST);

	// här ska object uppdateras om de ska röras eller nått
	
	// Update and Draw all objects
	for (int i = 0; i < OH->getNrOfObjects(); i++)
	{
		geometryPass->Update(OH->getObject(i)->GetTransform(), *camera);
		OH->getObject(i)->bindTexture();
		OH->getObject(i)->Draw();
	}

	glDisable(GL_DEPTH_TEST);
}

void DRLightPass(GBuffer *gBuffer, Mesh *fullScreenTriangle, GLuint *program, Shader *lightPass)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	lightPass->sendGBufferVariablesToGPU("gPosition", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	lightPass->sendGBufferVariablesToGPU("gDiffuse", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	lightPass->sendGBufferVariablesToGPU("gNormal", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	
	fullScreenTriangle->Draw();
}

// This function draws particles to the screen.
void particlePass(Particle * particle, Camera * camera, Shader * particleShader, float deltaTime)
{
	// We need the camera right/up vector and the camera location in world space to be able to make billboards out of the particles
	// PS = ParticleShader
	GLuint cameraRightWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraRightWorldPS");
	GLuint cameraUpWorldPS = glGetUniformLocation(*particleShader->getProgram(), "cameraUpWorldPS");
	GLuint viewProjection = glGetUniformLocation(*particleShader->getProgram(), "viewProjectionMatrix");

	// Create new particles
	particle->generateParticles(deltaTime);

	// Simulate all the particles
	particle->simulateParticles(camera->getCameraPosition(), deltaTime);

	// Update the buffers holding the particles
	particle->update();

	// Bind the correct shader
	particleShader->Bind();

	// Send Uniforms
	glUniform3f(cameraRightWorldPS, camera->getRightVector().x, camera->getRightVector().y, camera->getRightVector().z);
	glUniform3f(cameraUpWorldPS, camera->getUpVector().x, camera->getUpVector().y, camera->getUpVector().z);
	glUniformMatrix4fv(viewProjection, 1, GL_FALSE, &camera->getViewProjection()[0][0]);

	// Bind the buffers holding the particles
	particle->bind();

	// Draw the particles, send a draw call to the GPU
	particle->draw();

	// Unbind the shader
	particleShader->unBind();
}

void sendCameraLocationToGPU(GLuint cameraLocation, Camera *camera)
{
	glUniform3f(cameraLocation, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
}

void prepareTexture(GLuint textureLoc, GLuint normalMapLoc)
{
	glUniform1i(textureLoc, 0);
	glUniform1i(normalMapLoc, 1);
}

void setStartPositions(ObjectHandler * OH)
{
	// Initial positions for all cubes
	glm::vec3 cubePositions[2] =
	{
		glm::vec3(10.0f, 7.0f, -3.0f),
		glm::vec3(-7.0f, 7.0f, -3.0f)
	};

	// Transformations

	OH->getObject(cube1)->GetPos() = cubePositions[cube1];
	OH->getObject(cube2)->GetPos() = cubePositions[cube2];
	OH->getObject(sword)->GetPos() = glm::vec3(0.0f, 15.0f, 0.0f);
	OH->getObject(ground)->GetPos() = glm::vec3(0.0f, 0.0f, 0.0f);
	OH->getObject(moon)->GetPos() = glm::vec3(500.0f, 500.0f, 500.0f);
	OH->getObject(moon)->GetScale() = glm::vec3(100, 100, 100);

	OH->getObject(cube1)->GetScale() = glm::vec3(4, 3, 0.01);

	OH->getObject(sword)->GetRot().x = -(PI / 2);
	OH->getObject(sword)->GetRot().z = (PI / 16);
}

void keyboardControls(Display *display, Camera *camera)
{
	int keyboardButton;
	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to set the initial camera position & viewDir.
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_W);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveForward();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_S);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveBackward();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_D);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveRight();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_A);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveLeft();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_R);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveUp();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_F);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->moveDown();
	}
	keyboardButton = glfwGetKey(display->getWindow(), GLFW_KEY_SPACE);
	if (keyboardButton == GLFW_PRESS)
	{
		camera->setCameraPosition(camera->getStartCameraPosition());
		camera->setForwardVector(camera->getStartForwardVector());
	}
}

void mouseControls(Display *display, Camera *camera)
{
	double mouseXpos;
	double mouseYpos;
	int mouseState;

	// Check for mouse inputs, used to drag the camera view around
	mouseState = glfwGetMouseButton(display->getWindow(), GLFW_MOUSE_BUTTON_LEFT);

	// Find mouseposition (This function updates the X,Y values of the mouse position.
	glfwGetCursorPos(display->getWindow(), &mouseXpos, &mouseYpos);
	if (mouseState == GLFW_PRESS)
	{
		//std::cout << "pressed" << std::endl;
		camera->mouseUpdate(glm::vec2(mouseXpos, mouseYpos));
	}
	else if (mouseState == GLFW_RELEASE)
	{
	
	}
}