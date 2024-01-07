#include "BaseGameEngine.h"

BaseGameEngine::BaseGameEngine()
	: m_window(sf::VideoMode({ 1200, 800 }), "Minecraft Clone", sf::Style::Default, sf::ContextSettings{ 24, 0, 1, 3, 3, 0, false }) {
	m_window.setActive();
	m_window.setFramerateLimit(60u);
	m_window.setMouseCursorVisible(false);
}
