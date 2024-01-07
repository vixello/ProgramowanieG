#pragma once

#include <SFML/Graphics.hpp>

class BaseGameEngine {
public:
	BaseGameEngine();
	virtual ~BaseGameEngine() = default;

	virtual void Run() = 0;

protected:
	sf::RenderWindow m_window;
};