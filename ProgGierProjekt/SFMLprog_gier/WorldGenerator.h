//// WorldGenerator.h
//#pragma once
//#include "Chunk.h"
//#include "CubePalette.h"
//#include "glm/glm.hpp"
//#include <vector>
//
//class WorldGenerator {
//public:
//    WorldGenerator(CubePalette& palette);
//    void Update(const glm::vec2& playerPosition);
//    //void Update(const glm::vec3& playerPosition, const glm::vec3& viewDirection);
//    const std::vector<Chunk<16, 16, 32>>& GetGeneratedChunks() const;
//    Chunk<16, 16, 32>& getChunkAtPlayer(const glm::vec2& playerPosition, std::vector<Chunk<16, 16, 32>>& chunks);
//
//    glm::ivec2 playerChunk;
//private:
//    CubePalette& m_palette;
//    PerlinNoise rng;
//    std::vector<Chunk<16, 16, 32>> m_generatedChunks;
//    glm::vec2 m_lastPlayerPosition;
//    glm::vec2 m_lastPlayerChunk;
//    glm::vec2 m_lastViewChunk;
//    int CHUNK_RENDER_DISTANCE;
//};

#pragma once
#include "CubePalette.h"
#include "Chunk.h"
#include "PerlinNoise.h"
#include "Ray.h"
#include "Renderer.h"
#include "Entity.h"

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

#include <unordered_map>

class IStreamingSource {
public:
	virtual ~IStreamingSource() = default;

	virtual glm::vec3 GetPosition() const = 0;
};
//class CameraStreamingSource : public IStreamingSource {
//private:
//	const Camera& m_camera;
//	glm::vec3 m_position;
//public:
//	CameraStreamingSource(const Camera& camera) : m_camera(camera) {
//		m_position = m_camera.Position();  // Initialize the position
//	}
//
//	void UpdatePosition() {
//		m_position = m_camera.Position();  // Update the position
//	}
//
//	glm::vec3 GetPosition() const override {
//		return m_position;
//	}
//};
class IWorldGenerator {
public:
	struct HitRecord {
		glm::ivec3 m_cubeCoordinates;
		glm::ivec3 m_neighbourCoordinates;
		glm::ivec2 m_chunkCoordinates; 

	};

	virtual ~IWorldGenerator() = default;

	virtual void RegisterStreamingSource(IStreamingSource* source) = 0;
	virtual void UnregisterStreamingSource() = 0;

	virtual Ray::HitType Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const = 0;
	virtual void RemoveBlock(const glm::ivec3& coords) = 0;
	virtual void PlaceBlock(const glm::ivec3& coords, Cube::Type type) = 0;
};

class MinecraftClone;

class WorldGenerator : public IWorldGenerator, public IRenderable {
public:
	using FixedSizeChunk = Chunk<32, 32, 32>;

	WorldGenerator(MinecraftClone& instance, size_t renderDistance);
	~WorldGenerator();

	void Update(float deltaTime);

	template <class T, typename ...Args>
	T* CreateEntity(Args&&... params) {
		auto& ent = m_entities.emplace_back(std::make_unique<T>(m_instance, params...));

		return dynamic_cast<T*>(ent.get());
	}
	const std::unordered_map<glm::ivec2, Chunk<32, 32, 32>>& GetChunks() const {
		return m_chunks;
	}
	void Draw(ShaderProgram& shader) override;

	void RegisterStreamingSource(IStreamingSource* source) override;
	void UnregisterStreamingSource() override;

	Ray::HitType Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, IWorldGenerator::HitRecord& record) const override;
	void RemoveBlock(const glm::ivec3& coords) override;
	void PlaceBlock(const glm::ivec3& coords, Cube::Type type) override;

private:
	glm::ivec2 WorldCoordsToChunkOrigin(const glm::vec3& at) const;

	IStreamingSource* m_streamingSource{ nullptr };

	size_t m_renderDistance;
	std::unordered_map<glm::ivec2, Chunk<32, 32, 32>> m_chunks;
	CubePalette m_palette;
	PerlinNoise m_rng;
	std::vector<std::unique_ptr<Entity>> m_entities;

	MinecraftClone& m_instance;
};