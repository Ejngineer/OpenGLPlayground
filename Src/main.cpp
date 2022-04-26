#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

/*Globals*/
Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
double lastX = 0.0;
double lastY = 0.0;
bool firstMouse = true;
float fov = 45.0f;

struct MVP {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};


void DrawTree(float x, float y, float z, Shader& shader, unsigned int* VAO, MVP& mvp);

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

	/*Set callback for cursor controls*/
	glfwSetCursorPosCallback(window, mouse_callback);

	/*set scrool wheel callback*/
	glfwSetScrollCallback(window, scroll_callback);

	/*Capture mouse in window*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	const float Trunk[] =
	{
		 400.0f,  0.0f, 0.0f,
		 450.0f,  0.0f, 0.0f,
		 450.0f,  200.0f, 0.0f,

	     400.0f, 200.0f, 0.0f,
	     400.0f,   0.0f, 0.0f,
	     450.0f, 200.0f, 0.0f,

		 350.0f, 200.0f, 0.0f,
		 500.0f, 200.0f, 0.0f,
		 500.0f, 350.0f, 0.0f,

		 500.0f, 350.0f, 0.0f,
		 350.0f, 350.0f, 0.0f,
		 350.0f, 200.0f, 0.0f
	};

	const float quadVertices[] =
	{
		0.0f,   0.0f,   0.0f,
		800.0f, 0.0f,   0.0f,
		800.0f, 200.0f, 0.0f,

		800.0f, 200.0f, 0.0f,
		0.0f,   200.0f, 0.0f,
		0.0f, 0.0f,   0.0f

		//-800.0f, -200.0f, 0.0f,
		// 800.0f, -200.0f, 0.0f, 
		// 800.0f,  200.0f, 0.0f,

		// 800.0f,  200.0f, 0.0f,
		//-800.0f,  200.0f, 0.0f,
		//-800.0f, -200.0f, 0.0f
	};


	unsigned int VAO[2];
	unsigned int VBO[2];

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Trunk), Trunk, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shader_tri("shaders/basicshadervert.glsl", "shaders/basicshaderfrag.glsl");
	Shader shader_quad("shaders/basicshadervert.glsl", "shaders/basicshaderfrag.glsl");

	bool first = true;

	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		ProcessKeyInput(window, deltaTime);

		glClearColor(0.2f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_tri.use();

		MVP mvp;

		mvp.model = glm::mat4(1.0f);
		mvp.view =  glm::mat4(1.0f);
		mvp.view = glm::translate(mvp.view, glm::vec3(0.0f, 0.0f, -1.0f));
		//view = camera.GetViewMatrix();
		//glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		mvp.projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 1.0f, 100.0f);

		DrawTree(0.0f, 100.0f, 0.0f, shader_tri, VAO, mvp);
		DrawTree(150.0f, 25.0f, 0.0f, shader_tri, VAO, mvp);
		DrawTree(-250.0f, 50.0f, 0.0f, shader_tri, VAO, mvp);

		mvp.model = glm::translate(mvp.model, glm::vec3(0.0f, 0.0f, 0.0f));
		mvp.model = glm::scale(mvp.model, glm::vec3(1.0f));
		mvp.model = glm::mat4(1.0f);
		shader_tri.setMat4f("model", mvp.model);
		shader_tri.setMat4f("view", mvp.view);
		shader_tri.setMat4f("projection", mvp.projection);
		shader_tri.setVec3f("Color", glm::vec3(0.0f, 0.2f, 0.0f));
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



void DrawTree(float x, float y, float z, Shader& shader, unsigned int* VAO, MVP& mvp)
{
	mvp.model = glm::mat4(1.0f);
	mvp.model = glm::translate(mvp.model, glm::vec3(x, y, z));
	shader.use();
	shader.setMat4f("model", mvp.model);
	shader.setMat4f("view", mvp.view);
	shader.setMat4f("projection", mvp.projection);
	shader.setVec3f("Color", glm::vec3(0.35f, 0.25f, 0.12f));

	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	shader.setVec3f("Color", glm::vec3(0.0f, 0.5f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 6, 6);
	glBindVertexArray(0);

	/*mvp.model = glm::mat4(1.0f);
	mvp.model = glm::translate(mvp.model, glm::vec3(x - 0.59f, y, z + 0.1f));
	mvp.model = glm::scale(mvp.model, glm::vec3(0.5f));
	shader.setMat4f("model", mvp.model);
	shader.setMat4f("view", mvp.view);
	shader.setMat4f("projection", mvp.projection);
	shader.setVec3f("Color", glm::vec3(0.0f, 0.8f, 0.0f));

	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/
}