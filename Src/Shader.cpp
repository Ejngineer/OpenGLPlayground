#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{

		//std::string path1 = __FILE__; //gets source code path, include file name
		//path1 = path1.substr(0, 1 + path1.find_last_of('\\')); //removes file name
		//path1 += vertexPath; //adds input file to path
		//
		//std::string path2 = __FILE__; //gets source code path, include file name
		//path2 = path2.substr(0, 1 + path2.find_last_of('\\')); //removes file name
		//path2 += fragmentPath; //adds input file to path
		////open files
		//vShaderFile.open(path1);
		//fShaderFile.open(path2);

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geometryPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		std::stringstream gShaderStream;
		//read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();

		//close files
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();

		//store contents of stream into string variables
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR:::SHADER::FILE_NOT_SUCCESSFULLY_READ " << e.what() << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	unsigned int vertex;
	unsigned int fragment;
	unsigned int geometry;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	int success;
	char infoLog[512];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::FAILED::VERTEX_SHADER_CREATION\n" <<
			infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::FAILED::FRAGMENT_SHADER_CREATION\n" <<
			infoLog << std::endl;
	}

	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);

	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
		std::cout << "ERROR::FAILED::GEOMETRY_SHADER_CREATION\n" <<
			infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, nullptr, infoLog);
		std::cout << "ERROR::FAILED::PROGRAM_LINKING " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat1f(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat4(const std::string& name, float f0, float f1, float f2, float f3) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), f0, f1, f2, f3);
}

void Shader::setMat4f(const std::string& name, glm::mat4 transform) const
{
	int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::setVec3f(const std::string& name, glm::vec3 vec) const
{
	int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(transformLoc, vec.x, vec.y, vec.z);
}

void Shader::setVec3f(const std::string& name, float f0, float f1, float f2) const
{
	int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(transformLoc, f0, f1, f2);
}

unsigned int Shader::getID() const
{
	return ID;
}