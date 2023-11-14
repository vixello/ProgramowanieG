#pragma once
//#include <glm.hpp>
#include <SFML/Window.hpp>
#pragma once
#include "glm/glm.hpp" // vec2, vec3, mat4, radians
#include "glm/ext.hpp" // perspective, translate, rotate
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <random>
#include <iostream>
class Camera {
public:
	Camera(const glm::vec3& position, const glm::vec3& front, float yaw, float pitch);

	glm::mat4 View() const { return m_lookAt; }
	glm::mat4 Projection() const { return m_projection; }

	void Rotate(const sf::Vector2i& mouseDelta);
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);

private:
	void RecreateLootAt();

	glm::mat4 m_projection;
	glm::mat4 m_lookAt;

	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_front;
	glm::vec3 m_right;
	float m_yaw;
	float m_pitch;

	static glm::vec3 s_worldUp;
};