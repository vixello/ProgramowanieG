#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include "Shader.h"
//#include "CameraOld.h"
//#include "shaders.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Cube.h"
#include "Chunk.h"


int main()
{
    sf::ContextSettings contextSettings;

    bool sRgb = false;
    bool exit = false;

    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = sRgb;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;

    //sf::RenderWindow window(sf::VideoMode(800, 600), "Opengl hello world");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Minecraft", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    gladLoadGL();

    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    //std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    //std::cout << version;

    //glDepthFunc(GL_LESS);
    //glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);


    //Shader shader(VertexShader, FragmentShader);
    //CameraOld cameraOld;

    const uint8_t ChunkDepth = 8;
    const uint8_t ChunkWidth = 8;
    const uint8_t ChunkHeight = 32;

    ShaderProgram shaderProgram;
    Camera camera(glm::vec3(0.0f,20.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),-90.0f,0.0f);
    //Cube cube("grass.jpg");
    CubePalette cubePalette;

    std::vector<Chunk<ChunkDepth, ChunkWidth, ChunkHeight>> minecraftWorld;

    minecraftWorld.reserve(9);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            minecraftWorld.emplace_back(glm::vec2(i, j), cubePalette);
        }
    }
    PerlinNoise rng;
    for (auto& chunk : minecraftWorld) {
        chunk.Generate(rng);
    }

    ////vertex buffer object i vertex array object
    //GLuint vbo, vao;

    //std::pair<GLuint, GLuint> vertexObjectsPair = cube.CreateVertexBufferObject();

    //vbo = vertexObjectsPair.first;
    //vao = vertexObjectsPair.second;

    bool running = true;

    sf::Vector2i mousePosition = sf::Mouse::getPosition();
    sf::Clock clock;

    // run the program as long as the window is open


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
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //GLint uniModel = glGetUniformLocation(shaderProgram.getProgramId(), "model");
        //glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        //GLint uniView = glGetUniformLocation(shaderProgram.getProgramId(), "view");
        //glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.View()));

        //GLint uniProj = glGetUniformLocation(shaderProgram.getProgramId(), "proj");
        //glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.Projection()));

        const glm::mat4& view = camera.View();
        const glm::mat4& projection = camera.Projection();
        const glm::mat4 projectionView = projection * view;
        shaderProgram.SetMat4("projectionView", projectionView);
        //glDisable(GL_BLEND);
/*
        //glBindVertexArray(vao);
        glBindVertexArray(cube.Vao()); // Bind the cube's VAO

        glBindTexture(GL_TEXTURE_2D, cube.Texture());
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the cube
*/
        for (auto& chunk : minecraftWorld) {
            chunk.Draw(shaderProgram);
        }

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
