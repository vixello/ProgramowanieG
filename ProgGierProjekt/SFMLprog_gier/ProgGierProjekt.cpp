#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Shader.h"
#include "Camera.h"
#include "shaders.h"


std::pair<GLuint, GLuint> CreateVertexBufferObject() {
    const float triangle[] = {
        // x -> y -> z
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return std::make_pair(vbo, vao);
};
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

    std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::cout << version;

    //vertex buffer object i vertex array object

    GLuint vbo, vao;

    std::pair<GLuint, GLuint> vertexObjectsPair = CreateVertexBufferObject();

    vbo = vertexObjectsPair.first;
    vao = vertexObjectsPair.second;

    Shader shader(VertexShader, FragmentShader);
    Camera camera;


    bool running = true;

    // run the program as long as the window is open
    //while (window.isOpen())
    while (running)
    {
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


        // clear buffers 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glUseProgram(programId);
        shader.useProgram();

        GLint uniModel = glGetUniformLocation(shader.getProgramId(), "model");
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(camera.model));

        GLint uniView = glGetUniformLocation(shader.getProgramId(), "view");
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));

        GLint uniProj = glGetUniformLocation(shader.getProgramId(), "proj");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.projection));

        //nie wiem dlaczego glm::value_ptr(mvp) nie dzialalo, dlatego zamienilam na oddzielnie model view i projection
        //glm::mat4 mvp = camera.model * camera.view * camera.projection;


        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    /*
        glDeleteProgram(programId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        */
    return 0;
}
