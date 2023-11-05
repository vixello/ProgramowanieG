#include "Shader.h"


Shader::Shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource, const GLchar* geometryShaderSource )
	:programId(0), vertexShaderSource(vertexShaderSource), fragmentShaderSource(fragmentShaderSource), geometryShaderSource(0) {

    vertexShaderId = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShaderId = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    programId = createProgram(vertexShaderId, fragmentShaderId);
    
}
Shader::~Shader() {
    glDeleteProgram(programId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

//linkowanie programu
GLuint Shader::createProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
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
GLuint Shader::createShader(const GLchar* shaderSource, const GLenum shaderType)
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


GLuint Shader::getProgramId() {
    return programId;
}
void Shader::useProgram() {
    //turn on/use shader
    glUseProgram(programId);
}

