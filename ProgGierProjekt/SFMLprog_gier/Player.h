#pragma once

#include "Entity.h"
#include "WorldGenerator.h"
#include "Camera.h"

class Player : public Entity, public IStreamingSource {
public:
	Player(MinecraftClone& instance);
	~Player();

	void Update(float deltaTime) override;

	glm::vec3 GetPosition() const override;

private:
	bool m_leftButtonHeld{ false };
	bool m_rightButtonHeld{ false };
	sf::Vector2i m_mousePosition;
	void handleMouseInput(bool& buttonHeld, bool isLeftButton, const Camera& camera, IWorldGenerator* worldGenerator) ;

	Camera m_camera;

};