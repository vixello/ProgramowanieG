#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include "Shader.h"
//#include "CameraOld.h"
//#include "shaders.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Cube.h"


//std::pair<GLuint, GLuint> CreateVertexBufferObject() {
//    const float triangle[] = {
//        // x -> y -> z
//       -0.5f,-0.5f, 0.0f,
//        0.5f,-0.5f, 0.0f,
//        0.0f, 0.5f, 0.0f
//    };
//
//    float vertices[] = {
//         0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
//         0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
//        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
//        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
//    };
//    GLuint m_vbo, m_vao;
//    glGenVertexArrays(1, &m_vao);
//    glGenBuffers(1, &m_vbo);
//
//    glBindVertexArray(m_vao);
//    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // 3 floaty x y z reprezentujące pozycje w świecie gry
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // 2 floaty u v reprezentujące miejsce na teksturze
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    return std::make_pair(m_vbo, m_vao);
//};
int main()
{
    sf::ContextSettings contextSettings;

    bool sRgb = false;
    bool exit = false;

    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = sRgb;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Opengl hello world");
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    //std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    //std::cout << version;


    //Shader shader(VertexShader, FragmentShader);
    //CameraOld cameraOld;

    ShaderProgram shaderProgram;
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),-90.0f,0.0f);
    Cube cube("grass.jpg");

    ////vertex buffer object i vertex array object
    //GLuint vbo, vao;

    //std::pair<GLuint, GLuint> vertexObjectsPair = cube.CreateVertexBufferObject();

    //vbo = vertexObjectsPair.first;
    //vao = vertexObjectsPair.second;

    bool running = true;

    sf::Vector2i mousePosition = sf::Mouse::getPosition();
    sf::Clock clock;

    // run the program as long as the window is open

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    while (running)
    {
        // clear buffers 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float dt = clock.restart().asSeconds();

        const sf::Vector2i newMousePosition = sf::Mouse::getPosition();
        camera.Rotate(newMousePosition - mousePosition);
        mousePosition = newMousePosition;
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.MoveForward(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.MoveBackward(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.MoveLeft(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.MoveRight(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            camera.MoveUp(dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            camera.MoveDown(dt);
        }


        //glUseProgram(programId);
        shaderProgram.Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        GLint uniModel = glGetUniformLocation(shaderProgram.getProgramId(), "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        GLint uniView = glGetUniformLocation(shaderProgram.getProgramId(), "view");
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.View()));

        GLint uniProj = glGetUniformLocation(shaderProgram.getProgramId(), "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.Projection()));

        //glDisable(GL_BLEND);

        //glBindVertexArray(vao);
        glBindVertexArray(cube.Vao()); // Bind the cube's VAO

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the cube

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }
    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo);
    /*
        glDeleteProgram(programId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        */
    return 0;
}
