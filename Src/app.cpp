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
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 600);

	/*Let GLFW know we want to resize our viewport if the window is resized*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable opengl depth-testing
	glEnable(GL_DEPTH_TEST);

	//Enable Gamma correction
	//glEnable(GL_FRAMEBUFFER_SRGB);

	//Enable Multi-Sampling
	glEnable(GL_MULTISAMPLE);

	//Enable face culling
	//glEnable(GL_CULL_FACE);

	/*Set callback for cursor controls*/
	glfwSetCursorPosCallback(window, mouse_callback);

	/*set scrool wheel callback*/
	glfwSetScrollCallback(window, scroll_callback);

	/*Capture mouse in window*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_TEXTURE_2D);
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
		//model = glm::rotate(model, (float)glfwGetTime(), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		std::cout << glfwGetTime() << std::endl;
		model = glm::rotate(model, 5.0f, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		model = glm::rotate(model, -(float)glfwGetTime() * 0.1f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.f);
		/*VAO.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		VAO.UnBind();*/

		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}