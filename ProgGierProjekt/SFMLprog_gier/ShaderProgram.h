#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp" // vec2, vec3, mat4, radians
#include <string>

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& rhs) noexcept;
	ShaderProgram& operator=(ShaderProgram&& rhs) noexcept;
	~ShaderProgram();
	GLuint getProgramId() const {
		return m_programId;
	}

	void Use();


	void SetInt(const std::string_view name, int value);
	void SetMat4(const std::string_view name, const glm::mat4& value);

private:
	GLuint m_programId;

	static std::string s_vertexShaderSource;
	static std::string s_fragmentShaderSource;

	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
	GLuint createShader(const GLchar* shaderSource, const GLenum shaderType);

};