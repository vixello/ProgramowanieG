#include "Player.h"

#include "MinecraftClone.h"

Player::Player(MinecraftClone& instance)
	: Entity(instance)
    , m_camera(
        glm::vec3(0.5f, 16.0f, 0.5f),   // position
        glm::vec3(0.0f, 0.0f, 1.0f),    // front
        -90.0f,                         // yaw`
        0.0f                            // pitch
    ) {

    // Register camera
    m_instance.GetRenderer()->RegisterCamera(&m_camera);

    // Register streaming source
    m_instance.GetWorld()->RegisterStreamingSource(this);

}

Player::~Player() {
    // unregister camera
    m_instance.GetRenderer()->UnregisterCamera(&m_camera);
    // Unregister streaming source
    m_instance.GetWorld()->UnregisterStreamingSource();
}

void Player::Update(float deltaTime) {
    // keyboard related movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        // Move forward
        m_camera.MoveForward(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        // Move backward
        m_camera.MoveBackward(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        // Move left
        m_camera.MoveLeft(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        // Move right
        m_camera.MoveRight(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        // Move up
        m_camera.MoveUp(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        // Move down
        m_camera.MoveDown(deltaTime);
    }

    // mouse related movement
        // Get the current mouse position
    sf::Vector2i newMousePosition = sf::Mouse::getPosition();

    // Check if the window has focus and the mouse position is valid
    if (m_instance.GetRenderWindow().hasFocus() /*&&
        newMousePosition.x >= 0 && newMousePosition.y >= 0 &&
        newMousePosition.x < m_instance.GetRenderWindow().getSize().x &&
        newMousePosition.y < m_instance.GetRenderWindow().getSize().y*/)
    {
        // Rotate the camera based on mouse movement
        m_camera.Rotate(newMousePosition - m_mousePosition);
        m_mousePosition = newMousePosition;
    }
    else {
        std::cout << "WINDOW NOT FOCUSED" << std::endl;

    }
    // remove/place block
    handleMouseInput(m_leftButtonHeld, true, m_camera, m_instance.GetWorld());
    handleMouseInput(m_rightButtonHeld, false, m_camera, m_instance.GetWorld());

}

glm::vec3 Player::GetPosition() const {
    // for now it's ok if we treat camera's position as player's position
    return m_camera.Position();

}
void Player::handleMouseInput(bool& buttonHeld, bool isLeftButton, const Camera& camera, IWorldGenerator* worldGenerator) {
    if (sf::Mouse::isButtonPressed(isLeftButton ? sf::Mouse::Left : sf::Mouse::Right)) {
        if (!buttonHeld) {
            Ray ray(camera.Position(), camera.Direction());
            IWorldGenerator::HitRecord hitRecord;
            std::cout << "Camera Position: " << camera.Position().x << ", " << camera.Position().y << ", " << camera.Position().z << std::endl;

            // Use the WorldGenerator to perform block operations
            if (worldGenerator->Hit(ray, 0.0f, 4.0f, hitRecord) == Ray::HitType::Hit) {
                std::cout << "chunk was hit " << std::endl;

                // Perform block operations based on mouse button
                if (isLeftButton) {
                    // Remove block
                    worldGenerator->RemoveBlock(hitRecord.m_cubeCoordinates);
                    //worldGenerator->RemoveBlock(glm::ivec3(static_cast<int>(ray.Origin().x), static_cast<int>(ray.Origin().y), static_cast<int>(ray.Origin().z)));
                }
                else {
                    // Place block
                    worldGenerator->PlaceBlock(hitRecord.m_neighbourCoordinates, Cube::Type::Stone);
                }
            }
        }
        buttonHeld = true;
    }
    else {
        buttonHeld = false;
    }
}
