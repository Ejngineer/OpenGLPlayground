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

	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	/*Set callback for cursor controls*/
	glfwSetCursorPosCallback(window, mouse_callback);

	/*set scrool wheel callback*/
	glfwSetScrollCallback(window, scroll_callback);

	/*Capture mouse in window*/
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_TEXTURE_2D);

	float quadVertices[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	};

	VertexArray VAO;
	VertexBuffer VBO(quadVertices, sizeof(quadVertices));

	VAO.Bind();
	VBO.Bind();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	/*Geometries*/
	Sphere sphere(1.0f, 216, 216, true);
	Cube cube(1.0f, 1.0f, 1.0f);

	Shader shader("shaders/PBRvert.glsl", "shaders/PBRfrag.glsl");
	Shader shaderHDR("shaders/hdrvert.glsl", "shaders/hdrfrag.glsl");
	Shader cubeMapShader("shaders/cubemapvert.glsl", "shaders/cubemapfrag.glsl");
	Shader irradianceShader("shaders/convolutionvert.glsl", "shaders/convolutionfrag.glsl");
	Shader preFilterShader("shaders/hdrvert.glsl", "shaders/prefilterfrag.glsl");
	Shader brdfShader("shaders/brdfvert.glsl", "shaders/brdffrag.glsl");

	stbi_set_flip_vertically_on_load(1);

	Texture hdrTexture;
	hdrTexture.Bind2D();
	hdrTexture.LoadHdrMap("textures/hdr/newport_loft.hdr");

	Texture ironalbedo;
	Texture ironnorm;
	Texture ironao;
	Texture ironrough;
	Texture ironmetallic;

	stbi_set_flip_vertically_on_load(0);
	
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
	
	//ironalbedo.Bind2D();
	//ironalbedo.LoadTexture2D("textures/pbr/rounded-brick1-albedo.png");
	//ironnorm.Bind2D();
	//ironnorm.LoadTexture2D("textures/pbr/rounded-brick1-normal-ogl.png");
	//ironmetallic.Bind2D();
	//ironmetallic.LoadTexture2D("textures/pbr/rounded-brick1-metallic.png");
	//ironrough.Bind2D();
	//ironrough.LoadTexture2D("textures/pbr/rounded-brick1-roughness.png");
	//ironao.Bind2D();
	//ironao.LoadTexture2D("textures/pbr/rounded-brick1-ao.png");

	shader.use();
	shader.setInt("Albedo", 0);
	shader.setInt("normalMap", 1);
	shader.setInt("Metallic", 2);
	shader.setInt("Roughness", 3);
	shader.setInt("AO", 4);

	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	unsigned int envCubeMap;
	glGenTextures(1, &envCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureView[] =
	{
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	shaderHDR.use();
	shaderHDR.setInt("hdrTexture", 0);
	shaderHDR.setMat4f("projection", captureProjection);
	hdrTexture.ActivateTexture(0);
	hdrTexture.Bind2D();

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; i++)
	{
		shaderHDR.setMat4f("view", captureView[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4f("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; i++)
	{
		irradianceShader.setMat4f("view", captureView[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	preFilterShader.use();
	preFilterShader.setMat4f("projection", captureProjection);
	preFilterShader.setInt("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; mip++)
	{
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		preFilterShader.setFloat1f("roughness", roughness);

		for (unsigned int i = 0; i < 6; i++)
		{
			preFilterShader.setMat4f("view", captureView[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cube.Draw();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);

	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	VAO.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
	VAO.UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	cubeMapShader.use();
	cubeMapShader.setInt("cubeMap", 0);

	shader.use();
	shader.setInt("irradianceMap", 5);
	shader.setInt("prefilterMap", 6);
	shader.setInt("brdfLUT", 7);

	glm::vec3 lightPositions[] =
	{
		glm::vec3( 2.0f, 3.0f, 5.0f),
		glm::vec3( 3.0f,-3.0f, 50.0f),
		glm::vec3( 3.0f, 3.0f, 50.0f),
		glm::vec3(-3.0f, 3.0f, 50.0f),
	};

	glm::vec3 lightColors[] =
	{
		glm::vec3(300.0f,300.0f,300.0f),
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
		//shader.setVec3f("Albedo", glm::vec3(0.0f, 0.5f,0.0f));
		//shader.setFloat1f("Metallic", 0.9f);
		//shader.setFloat1f("Roughness", 0.8f);
		//shader.setFloat1f("AO", 1.0f);
		shader.setVec3f("camPos", camera.GetPosition());
		shader.setFloat1f("Divisions", 2.0f);

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

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

		sphere.Draw();
		
		//model = glm::mat4(1.0f);
		//for (int row = 0; row < nrRows; ++row)
		//{
		//	shader.setFloat1f("Metallic", (float)row / (float)nrRows);
		//	for (int col = 0; col < nrColumns; ++col)
		//	{
		//		// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
		//		// on direct lighting.
		//		shader.setFloat1f("Roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
		//
		//		model = glm::mat4(1.0f);
		//		model = glm::translate(model, glm::vec3(
		//			(float)(col - (nrColumns / 2)) * spacing,
		//			(float)(row - (nrRows / 2)) * spacing,
		//			-2.0f
		//		));
		//		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//		shader.setMat4f("model", model);
		//		sphere.Draw();
		//	}
		//}

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

		cubeMapShader.use();
		cubeMapShader.setMat4f("view", view);
		cubeMapShader.setMat4f("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
		cube.Draw();

		//brdfShader.use();
		//VAO.Bind();
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
		//VAO.UnBind();

		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}