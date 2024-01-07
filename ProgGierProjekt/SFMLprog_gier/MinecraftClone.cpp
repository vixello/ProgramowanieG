#include "MinecraftClone.h"

#include "Player.h"

MinecraftClone::MinecraftClone()
    :
    m_world(*this, 1) {
    m_world.CreateEntity<Player>();
}

void MinecraftClone::Run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        const float dt = clock.restart().asSeconds();
        //std::cout << "WINDOW OPENED" << std::endl;
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        m_world.Update(dt);
        m_renderer.Draw();
        m_window.display();
    }
}

IRenderer* MinecraftClone::GetRenderer() {
    return &m_renderer;
}

IWorldGenerator* MinecraftClone::GetWorld() {
    return &m_world;
}


