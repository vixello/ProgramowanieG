#pragma once

#include "BaseGameEngine.h"
#include "WorldGenerator.h"
#include "Renderer.h"

class MinecraftClone : public BaseGameEngine {
public:
	MinecraftClone();
	void Run() override;

	IRenderer* GetRenderer();
	IWorldGenerator* GetWorld();
	sf::RenderWindow& GetRenderWindow() {
		return m_window;
	}
private:
	Renderer m_renderer;
	WorldGenerator m_world;
	//sf::RenderWindow m_window;

};