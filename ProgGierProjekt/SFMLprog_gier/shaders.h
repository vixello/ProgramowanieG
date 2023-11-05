
#pragma once
//const GLchar* VertexShader = R"(
//    #version 330 core
//    layout (location = 0) in vec3 position;
//    void main() {
//        gl_Position = vec4(position.x, position.y, position.z, 1.0);
//    }
//    )";
const GLchar* VertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 mvp;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main() {
        gl_Position = proj * view * model * vec4(aPos, 1.0);
    }
    )";
const GLchar* FragmentShader = R"(
    #version 330 core
    out vec4 fragmentColor;

    void main() {
        fragmentColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
    )";

