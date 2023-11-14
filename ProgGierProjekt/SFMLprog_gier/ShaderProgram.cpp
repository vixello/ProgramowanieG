#include "ShaderProgram.h"

#include <iostream>
#include <assert.h>

std::string ShaderProgram::s_vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 inTexCoords;
    
    out vec2 outTexCoords;
    
    uniform mat4 mvp;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() {
        gl_Position = proj * view * model * vec4(aPos, 1.0);
        outTexCoords = inTexCoords;
    }
    )";

std::string ShaderProgram::s_fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 outTexCoords;

    uniform sampler2D texture1;

    void main() {
        FragColor = texture(texture1, outTexCoords);
    }
    )";

ShaderProgram::ShaderProgram(ShaderProgram&& rhs) noexcept
	: m_programId(std::exchange(rhs.m_programId, 0)) {
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}

	m_programId = std::exchange(rhs.m_programId, 0);

	return *this;
}

ShaderProgram::ShaderProgram() : m_programId(glCreateProgram()) {

    const GLuint vertexShader = createShader(s_vertexShaderSource.c_str(), GL_VERTEX_SHADER);
    const GLuint fragmentShader = createShader(s_fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

    m_programId = createProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Use();
    SetInt("texture1", 0);
}
ShaderProgram::~ShaderProgram() {
}
GLuint ShaderProgram::createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
    int success;
    char infoLog[512];

    const GLuint programId = glCreateProgram();
    if (!programId) {
        return 0;
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    if (geometryShader) glAttachShader(programId, geometryShader);

    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(programId, 512, NULL, infoLog);
        std::cout << "\nError: program [" << programId << "] linking failed\n" << infoLog << "\n";
    }
    return programId;
}

//kompilacja shaderow
GLuint ShaderProgram::createShader(const GLchar* shaderSource, const GLenum shaderType)
{
    int success;
    char infoLog[512];
    const GLuint shaderId = glCreateShader(shaderType);

    if (!shaderId) {
        return 0;
    }
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    //error handling

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "\nError: shader [" << shaderId << "] compilation failed\n" << infoLog << "\n";
    }
    return shaderId;
};

void ShaderProgram::Use() {
    glUseProgram(m_programId);
}

void ShaderProgram::SetInt(const std::string_view name, int value) {
    glUniform1i(glGetUniformLocation(m_programId, name.data()), value);
}

void ShaderProgram::SetMat4(const std::string_view name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.data()), 1, GL_FALSE, &value[0][0]);
}

