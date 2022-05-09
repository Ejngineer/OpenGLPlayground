#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include "stb_image.h"
#include "Cube.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Sphere.h"

/*Globals*/
Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
double lastX = 0.0;
double lastY = 0.0;
bool firstMouse = true;
float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement((float)xoffset, (float)yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(fov, (float)yoffset);
}

void ProcessKeyInput(GLFWwindow* window ,float deltatime) 
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltatime);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1200, 900, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*Load extension loader library*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*Set size of window where our graphics will be shown*/
	glViewport(0, 0, 1200, 900);

	/*Let GLFW know we want to resize our viewport if the window is resized*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable opengl depth-testing
	glEnable(GL_DEPTH_TEST);

	/*Set callback for cursor controls*/
	glfwSetCursorPosCallback(window, mouse_callback);

	/*set scrool wheel callback*/
	glfwSetScrollCallback(window, scroll_callback);

	/*Capture mouse in window*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_TEXTURE_2D);

	Sphere sphere(1.0f, 144, 144);
	Shader shader("shaders/PBRvert.glsl", "shaders/PBRfrag.glsl");
	Shader shader1("shaders/spherevert.glsl", "shaders/spherefrag.glsl");

	stbi_set_flip_vertically_on_load(1);

	Texture ironalbedo;
	Texture ironnorm;
	Texture ironao;
	Texture ironrough;
	Texture ironmetallic;

	ironalbedo.Bind2D();
	ironalbedo.LoadTexture2D("textures/pbr/albedo.png");
	ironnorm.Bind2D();
	ironnorm.LoadTexture2D("textures/pbr/normal.png");
	ironmetallic.Bind2D();
	ironmetallic.LoadTexture2D("textures/pbr/metallic.png");
	ironrough.Bind2D();
	ironrough.LoadTexture2D("textures/pbr/roughness.png");
	ironao.Bind2D();
	ironao.LoadTexture2D("textures/pbr/ao.png");

	shader.use();
	shader.setInt("Albedo", 0);
	shader.setInt("normalMap", 1);
	shader.setInt("Metallic", 2);
	shader.setInt("Roughness", 3);
	shader.setInt("AO", 4);

	glm::vec3 lightPositions[] =
	{
		glm::vec3( 0.0f, 0.0f, 10.0f),
		glm::vec3( 3.0f,-3.0f, 50.0f),
		glm::vec3( 3.0f, 3.0f, 50.0f),
		glm::vec3(-3.0f, 3.0f, 50.0f),
	};

	glm::vec3 lightColors[] =
	{
		glm::vec3(250.0f,250.0f,250.0f),
		glm::vec3(150.0f,150.0f,150.0f),
		glm::vec3(150.0f,150.0f,150.0f),
		glm::vec3(150.0f,150.0f,150.0f),
	};

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5f;


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessKeyInput(window, deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(fov), 1200.0f / 900.0f, 0.1f, 100.f);
	
		shader.use();
		shader.setMat4f("model", model);
		shader.setMat4f("view", view);
		shader.setMat4f("projection", projection);
		//shader.setVec3f("Albedo", glm::vec3(0.5f, 0.0f,0.0f));
		//shader.setFloat1f("Metallic", 0.2f);
		//shader.setFloat1f("Roughness", 0.2f);
		//shader.setFloat1f("AO", 0.2f);
		shader.setVec3f("camPos", camera.GetPosition());

		/*Setup textures*/
		ironalbedo.ActivateTexture(0);
		ironalbedo.Bind2D();
		ironnorm.ActivateTexture(1);
		ironnorm.Bind2D();
		ironmetallic.ActivateTexture(2);
		ironmetallic.Bind2D();
		ironrough.ActivateTexture(3);
		ironrough.Bind2D();
		ironao.ActivateTexture(4);
		ironao.Bind2D();

		sphere.Draw();

		for (int i = 0; i < 1; i++)
		{
			float lightPos = lightPositions[i].x;// +sinf(glfwGetTime()) * 5.0f;
			shader.setVec3f("lightPositions[" + std::to_string(i) + "]", glm::vec3(lightPos, lightPositions[i].y, lightPositions[i].z));
			shader.setVec3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(lightPos, lightPositions[i].y, lightPositions[i].z));
			model = glm::scale(model, glm::vec3(0.5f));
			//shader.setMat4f("model", model);
			//shader.setVec3f("Albedo", lightColors[i]);
			//sphere.Draw();
		}

		//shader1.use();
		//model = glm::mat4(1.0f);
		//shader1.setMat4f("model", model);
		//shader1.setMat4f("view", view);
		//shader1.setMat4f("projection", projection);
		//sphere.Draw();

		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}