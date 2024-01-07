#pragma once

#include "ShaderProgram.h"
#include "Camera.h"

#include <vector>

class IRenderable {
public:
	virtual ~IRenderable() = default;

	virtual void Draw(ShaderProgram& program) = 0;
};

class IRenderer {
public:
	virtual ~IRenderer() = default;

	virtual void RegisterCamera(Camera* camera) = 0;
	virtual void UnregisterCamera(Camera* camera) = 0;
	
	virtual void RegisterRenderable(IRenderable* renderable) = 0;
	virtual void UnregisterRenderable(IRenderable* renderable) = 0;
};

class Renderer : public IRenderer {
public:
	Renderer();

	void Draw();

	void RegisterCamera(Camera* camera) override;
	void UnregisterCamera(Camera* camera) override;

	void RegisterRenderable(IRenderable* renderable) override;
	void UnregisterRenderable(IRenderable* renderable) override;

private:
	std::unique_ptr<ShaderProgram> m_shader;

	Camera* m_camera{ nullptr };
	std::vector<IRenderable*> m_renderables;
};