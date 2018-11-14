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
// Finns en main funktion i GLEW, därmed måste vi undefinera den innan vi kan använda våran main
#undef main

#define PI 3.1415926535

int SCREENWIDTH = 800;
int SCREENHEIGHT = 600;

// Deferred Rendering Functions
void DRGeometryPass(GBuffer *gBuffer, double counter, Object objects[], Shader *geometryPass, Camera *camera, Object *triangleTest);
void DRLightPass(GBuffer *gBuffer, Mesh *fullScreenQuad, GLuint *program, Shader *geometryPass);

void sendCameraToGPU(GLuint cameraLocation, Camera *camera);
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

	geometryPass.initiateShaders();
	lightPass.initiateShaders();
	
	Camera camera(glm::vec3(0, 0, -6), 70.0f,(float)SCREENWIDTH / (float)SCREENHEIGHT, 0.01f, 1000.0f);
	
	Vertex verticesTriangleTest[] =
	{	
		Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0,0.0)),
		Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5,1.0)),
		Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0,0.0))
	};

	Vertex verticesPlane[] =
	{
		Vertex(glm::vec3(-1.0f, -1.0f, 0), glm::vec2(0.0f,  0.0f)), // bottom-left
		Vertex(glm::vec3(1.0f,  1.0f, 0), glm::vec2(0.5f,  1.0f)), // top-right
		Vertex(glm::vec3(1.0f, -1.0f, 0), glm::vec2(1.0f,  0.0f)), // bottom-right         
		Vertex(glm::vec3(1.0f,  1.0f, 0), glm::vec2(0.0f,  0.0f)), // top-right
		Vertex(glm::vec3(-1.0f, -1.0f, 0), glm::vec2(0.5f,  1.0f)), // bottom-left
		Vertex(glm::vec3(-1.0f,  1.0f, 0), glm::vec2(1.0f,  0.0f)), // top-left
	};
	
	Mesh planeMesh(verticesPlane, sizeof(verticesPlane) / sizeof(verticesPlane[0]));

	Vertex vertices[] = {
		// back face
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f,  0.0f)), // bottom-left
		Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(0.5f,  1.0f)), // top-right
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f,  0.0f)), // bottom-right         
		Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(0.0f,  0.0f)), // top-right
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.5f,  1.0f)), // bottom-left
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(1.0f,  0.0f)), // top-left
		// front face
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f,  0.0f)), // bottom-left
		Vertex(glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(0.5f,  1.0f)), // bottom-right
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(1.0f,  0.0f)), // top-right
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(0.0f,  0.0f)), // top-right
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.5f,  1.0f)), // top-left
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(1.0f,  0.0f)), // bottom-left
		// left face
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f,  0.0f)),  // top-right
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.5f,  1.0f)),  // top-left
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f,  0.0f)),  // bottom-left
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f,  0.0f)),  // bottom-left
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.5f,  1.0f)),  // bottom-right
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(1.0f,  0.0f)),  // top-right
		// right face
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(0.0f,  0.0f)),  // top-left
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.5f,  1.0f)),  // bottom-right
		Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(1.0f,  0.0f)),  // top-right         
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f,  0.0f)),  // bottom-right
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(0.5f,  1.0f)),  // top-left
		Vertex(glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(1.0f,  0.0f)),  // bottom-left     
		// bottom face
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f,  0.0f)),  // top-right
		Vertex(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.5f,  1.0f)),  // top-left
		Vertex(glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(1.0f,  0.0f)),  // bottom-left
		Vertex(glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(0.0f,  0.0f)),  // bottom-left
		Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.5f,  1.0f)),  // bottom-right
		Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f,  0.0f)),  // top-right
		// top face
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f,  0.0f)),  // top-left
		Vertex(glm::vec3(1.0f,  1.0f , 1.0f), glm::vec2(0.5f,  1.0f)),  // bottom-right
		Vertex(glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(1.0f,  0.0f)),  // top-right     
		Vertex(glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(0.0f,  0.0f)),  // bottom-right
		Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.5f,  1.0f)),  // top-left
		Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(1.0f,  0.0f))  // bottom-left        
	};

	// Ett Object med mesh, transform och texture
	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
	Transform transform;
	Texture texture("Textures/basicSnow.jpg");

	Mesh triangleTestMesh(verticesTriangleTest, sizeof(verticesTriangleTest) / sizeof(verticesTriangleTest[0]));
	Transform transformTriangleTest;

	ObjectHandler OH = ObjectHandler();



	Mesh meshOBJ;
	Object objects[12];
	for (int i = 0; i < 12; i++)
	{
		objects[i] = OH.CreateObject(&mesh, transform, &texture);
		if (i == 10)
		{
			objects[i] = OH.CreateObject(&planeMesh, transform, &texture);
		}
		if (i == 11)
		{
			objects[i] = OH.CreateObject("ObjectFiles/srd.obj",&meshOBJ, transform, &texture);
		}
	}
	
	Object triangleTest = OH.CreateObject(&triangleTestMesh, transformTriangleTest, &texture);
	

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
	int nrOfFrames = 0;
	double deltaTime = 0;

	// Create Lights
	PointLightHandler lights(MAX_NUMBER_OF_LIGHTS);
	lights.setLight(0, glm::vec3(-3.0f, -1.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.setLight(1, glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lights.initiateLights(lightPass.getProgram());
	
	// Tell the shaders the name of the camera (GP = GeometeryPass, LP = LightPass)
	GLuint cameraLocationGP = glGetUniformLocation(*geometryPass.getProgram(), "cameraPosGP");
	GLuint cameraLocationLP = glGetUniformLocation(*lightPass.getProgram(), "cameraPosLP");
	while(!display.IsWindowClosed())
	{
		deltaTime = currentTime - lastTime;
		lastTime = glfwGetTime();

		geometryPass.Bind();
		// Kanske ska läggas in i en loop ifall vi senare har flera textures
		texture.Bind(0);

		sendCameraToGPU(cameraLocationGP, &camera);
		// Här inne sker all rotation och sånt på alla meshes
		DRGeometryPass(&gBuffer, counter, objects, &geometryPass, &camera, &triangleTest);
		geometryPass.unBind();

		lightPass.Bind();

		lights.sendToShader();
		sendCameraToGPU(cameraLocationLP, &camera);
		// Här inne renderas en fullscreenTriangle och lights skickas till GPU
		DRLightPass(&gBuffer, &fullScreenTriangle, lightPass.getProgram(), &lightPass);
		lightPass.unBind();

		// Check for mouse/keyboard inputs and handle the camera movement
		mouseControls(&display, &camera);
		keyboardControls(&display, &camera);

		camera.updateViewMatrix();

		display.SwapBuffers(SCREENWIDTH, SCREENHEIGHT);

		counter += deltaTime * 0.5;
		currentTime = glfwGetTime();
	}
	return 0;
}

void DRGeometryPass(GBuffer *gBuffer, double counter, Object objects[], Shader *geometryPass, Camera *camera, Object *triangleTest)
{
	gBuffer->BindForWriting();

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//EVENTUELLT gör detta till egen funktion i Display
	
	glEnable(GL_DEPTH_TEST);

	// Initial positions for all cubes
	glm::vec3 cubePositions[10] =
	{
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(5.0f,  3.0f, -6.0f),
		glm::vec3(-2.5f, -3.2f, -3.5f),
		glm::vec3(-4.8f, -3.0f, -2.3f),
		glm::vec3(5.0f, -1.4f, -4.5f),
		glm::vec3(-2.7f,  3.0f, -5.5f),
		glm::vec3(2.3f, -3.0f, -6.5f),
		glm::vec3(2.5f,  -5.0f, -4.5f),
		glm::vec3(2.5f,  1.2f, -2.5f),
		glm::vec3(-2.3f,  2.0f, -2.5f)
	};

	// Cube transformations
	for (int i = 0; i < 10; i++)
	{
		objects[i].GetPos() = cubePositions[i];
		objects[i].GetScale() = glm::vec3(0.6f, 0.6f, 0.6f);

		// Cube movement
		if (i < 5)
		{
			objects[i].GetPos().x = sinf(counter*2.3f);
		}
		else
		{
			objects[i].GetPos().y = sinf(counter);
		}

		// Rotation
		if (i < 3)
		{
			objects[i].GetRot().x = sinf(counter);
		}
		else if (i > 3 && i < 7)
		{
			objects[i].GetRot().y = sinf(counter);
			objects[i].GetRot().z = sinf(counter);
		}
		else
		{
			objects[i].GetRot().x = sinf(counter);
			objects[i].GetRot().y = sinf(counter);
		}
	}
	// Plane
	objects[10].GetRot().x = PI/2;
	objects[10].GetScale() = glm::vec3(10.0f, 10.0f, 1.0f);
	objects[10].GetPos().y = -4;
	
	// ------------ När vi har flera så skall detta vara inom en loop ------------
	for (int i = 0; i < 12; i++)
	{
		geometryPass->Update(objects[i].GetTransform(), *camera);
		objects[i].Draw();
	}
	
	geometryPass->Update(triangleTest->GetTransform(), *camera);
	triangleTest->Draw();
	// ----------------------------------------------------------------------------
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

void InitiateLightPass(GBuffer *gbuffer)
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	gbuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}

void sendCameraToGPU(GLuint cameraLocation, Camera *camera)
{
	glUniform3f(cameraLocation, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
}

void keyboardControls(Display *display, Camera *camera)
{
	int keyboardButton;
	// Check for keyboard inputs, used to move the camera around.
	// WASD for movearound, RF (Rise,Fall) and space to locate the initial camera position.
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