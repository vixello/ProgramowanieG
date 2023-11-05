#pragma once
#include <glad/glad.h>
#include <iostream>


class Shader
{
public:
	Shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource, const GLchar* geometryShaderSource = 0);
	~Shader();

	void useProgram(); // This method activates the shader program
	//void UpdateCameraPosition(glm::vec3& cameraPosition, sf::Clock& clock);
	GLuint getProgramId();
	//glm::vec3 cameraPosition;
	//glm::vec3 getRandomCameraPosition();

private:
	GLuint programId;
	const GLchar* vertexShaderSource;
	const GLchar* fragmentShaderSource;
	const GLchar* geometryShaderSource;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	GLuint createShader(const GLchar* shaderSource, const GLenum shaderType);
	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
};

