#pragma once

class MinecraftClone;

class Entity {
public:
	Entity(MinecraftClone& instance);
	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;

protected:
	MinecraftClone& m_instance;
};