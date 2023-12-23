#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
//#include "Shader.h"
//#include "CameraOld.h"
//#include "shaders.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Cube.h"
#include "Chunk.h"
#include "WorldGenerator.h"

//void handleMouseInput(bool& buttonHeld, bool isLeftButton, const Camera& camera, std::vector<Chunk<32, 32, 32>>& minecraftWorld) {
//    if (sf::Mouse::isButtonPressed(isLeftButton ? sf::Mouse::Left : sf::Mouse::Right)) {
//        if (!buttonHeld) {
//            Ray ray(camera.Position(), camera.Direction());
//            Chunk<32, 32, 32>::HitRecord hitRecord;
//            for (auto& chunk : minecraftWorld) {
//                if (chunk.Hit(ray, 0.0f, 4.0f, hitRecord) == Ray::HitType::Hit) {
//                    if (isLeftButton) {
//                        chunk.RemoveBlock(hitRecord.m_cubeIndex.x, hitRecord.m_cubeIndex.y, hitRecord.m_cubeIndex.z);
//                    }
//                    else {
//                        std::cout << "chunk hit" << std::endl;
//                        chunk.PlaceBlock(hitRecord.m_neighbourIndex.x, hitRecord.m_neighbourIndex.y, hitRecord.m_neighbourIndex.z, Cube::Type::Stone);
//                    }
//                }
//            }
//        }
//        buttonHeld = true;
//    }
//    else {
//        buttonHeld = false;
//    }
//}
void handleMouseInput(bool& buttonHeld, bool isLeftButton, const Camera& camera, WorldGenerator& worldGenerator) {
    if (sf::Mouse::isButtonPressed(isLeftButton ? sf::Mouse::Left : sf::Mouse::Right)) {
        if (!buttonHeld) {
            Ray ray(camera.Position(), camera.Direction());
            IWorldGenerator::HitRecord hitRecord;
            std::cout << "Camera Position: " << camera.Position().x << ", " << camera.Position().y << ", " << camera.Position().z << std::endl;

            // Use the WorldGenerator to perform block operations
            if (worldGenerator.Hit(ray, 0.0f, 4.0f, hitRecord) == Ray::HitType::Hit) {
                std::cout << "chunk was hit " << std::endl;

                // Perform block operations based on mouse button
                if (isLeftButton) {
                    // Remove block
                    worldGenerator.RemoveBlock(hitRecord.m_cubeCoordinates);
                }
                else {
                    // Place block
                    worldGenerator.PlaceBlock(hitRecord.m_neighbourCoordinates, Cube::Type::Stone);
                }
            }
        }
        buttonHeld = true;
    }
    else {
        buttonHeld = false;
    }
}

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

    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Minecraft", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    gladLoadGL();

    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    //glDepthFunc(GL_LESS);
    //glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);


    const uint8_t ChunkDepth = 16;
    const uint8_t ChunkWidth = 16;
    const uint8_t ChunkHeight = 32;

    ShaderProgram shaderProgram;
    Camera camera(glm::vec3(0.0f,18.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),0.0f,0.0f);
    //Cube cube("grass.jpg");
    CubePalette cubePalette;

    //std::vector<Chunk<ChunkDepth, ChunkWidth, ChunkHeight>> minecraftWorld;

    //minecraftWorld.reserve(1);
    //for (int i = 0; i < 3; i++) {
    //    for (int j = 0; j < 3; j++) {
    //        minecraftWorld.emplace_back(glm::vec2(i, j), cubePalette);
    //    }
    //}
    PerlinNoise rng;
    //for (auto& chunk : minecraftWorld) {
    //    chunk.Generate(rng);
    //}
    size_t renderDistance = 1;  

    WorldGenerator worldGenerator(renderDistance);

    bool running = true;
    bool rbHeld = false;
    bool lbHeld = false;

    sf::Vector2i mousePosition = sf::Mouse::getPosition();
    sf::Clock clock;
    CameraStreamingSource cameraStreamingSource(camera);
    worldGenerator.RegisterStreamingSource(&cameraStreamingSource);

    // run the program as long as the window is open
    while (running)
    {
        // clear buffers 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float dt = clock.restart().asSeconds();

        if (window.hasFocus()) {
            const sf::Vector2i newMousePosition = sf::Mouse::getPosition(window);
            if (newMousePosition.x >= 0 && newMousePosition.y >= 0 &&
                newMousePosition.x < window.getSize().x && newMousePosition.y < window.getSize().y)
            {
                camera.Rotate(newMousePosition - mousePosition);
                mousePosition = newMousePosition;
            }
        }




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
        //std::cout << camera.Direction().x<<" "<< camera.Direction().y<<" "<< camera.Direction().z << std::endl;

        // Handle mouse input for left button
        handleMouseInput(lbHeld, true, camera, worldGenerator );

        // Handle mouse input for right button
        handleMouseInput(rbHeld, false, camera, worldGenerator);

        //glUseProgram(programId);
        shaderProgram.Use();

        cameraStreamingSource.UpdatePosition();
        worldGenerator.Update(dt);

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

        //for (auto& chunk : minecraftWorld) {
        //    chunk.Draw(shaderProgram);
        //}

        for (const auto& chunkEntry : worldGenerator.GetChunks()) {
            const Chunk<32, 32, 32>& chunk = chunkEntry.second;
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

