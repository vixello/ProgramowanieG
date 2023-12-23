
#include "WorldGenerator.h"
//#include "Player.h"
//#include "MinecraftClone.h"

#include <cmath>
#include <algorithm>
#include <iterator>

WorldGenerator::WorldGenerator(/*MinecraftClone& instance,*/ size_t renderDistance)
	: m_renderDistance(renderDistance)
	/*, m_instance(instance)*/ {
	// register renderable
}

WorldGenerator::~WorldGenerator() {
	// unregister renderable
}

void WorldGenerator::Update(float deltaTime) {
	//// Update all entities
	//for (auto& entity : m_entities) {
	//	entity->Update(deltaTime);
	//}

	// lexigraphical comparator
	// accept any 2D vector of values implementing order operator
	// comes in handy when using sort and set_difference
	static auto comp = [](const auto& a, const auto& b) -> bool {
		if (a[0] == b[0]) {
			return a[1] < b[1];
		}
		else {
			return a[0] < b[0];
		}
	};


	// Get streaming position
	glm::vec3 streamingPosition = m_streamingSource ? m_streamingSource->GetPosition() : glm::vec3(0.0f);
	if (m_streamingSource) {
		static_cast<CameraStreamingSource*>(m_streamingSource)->UpdatePosition();
	}
	//std::cout << "streamingPosition: (" << streamingPosition.x << ", " << streamingPosition.y << ", " << streamingPosition.z << ")" << std::endl;

	// Get center chunk
	glm::ivec2 centerChunk = WorldCoordsToChunkOrigin(streamingPosition);

	// Prepare array of ivec2s
	std::vector<glm::ivec2> chunkArray;
	chunkArray.reserve(m_chunks.size());
	std::transform(m_chunks.begin(), m_chunks.end(), std::back_inserter(chunkArray),
		[](const auto& pair) { return pair.first; });

	// Sort array
	std::sort(chunkArray.begin(), chunkArray.end(), comp);

	// Prepare array of neighbour chunks indices (ivec2)
	std::vector<glm::ivec2> neighbourChunks;

	for (int xOffset = -static_cast<int>(m_renderDistance); xOffset <= static_cast<int>(m_renderDistance); ++xOffset) {
		for (int yOffset = -static_cast<int>(m_renderDistance); yOffset <= static_cast<int>(m_renderDistance); ++yOffset) {
			neighbourChunks.emplace_back(centerChunk.x + xOffset, centerChunk.y + yOffset);
		}
	}
	// ChunksToRemove is a set_difference between current chunks and neighbour chunks
	std::vector<glm::ivec2> chunksToRemove;
	std::set_difference(chunkArray.begin(), chunkArray.end(),
		neighbourChunks.begin(), neighbourChunks.end(),
		std::back_inserter(chunksToRemove), comp);

	// Erase those chunks
	for (const auto& toRemove : chunksToRemove) {
		m_chunks.erase(toRemove);
	}

	//// ChunksToAdd is a set_difference between neighbours chunks and current chunks
	//std::vector<glm::ivec2> chunksToAdd;
	//std::set_difference(neighbourChunks.begin(), neighbourChunks.end(),
	//	chunkArray.begin(), chunkArray.end(),
	//	std::back_inserter(chunksToAdd), comp);
// Chunks to add is a set_difference between neighbours chunk and current chunks
	std::vector<glm::ivec2> chunksToAdd;

	for (const auto& neighbour : neighbourChunks) {
		auto it = std::find(chunkArray.begin(), chunkArray.end(), neighbour);
		if (it == chunkArray.end()) {
			chunksToAdd.push_back(neighbour);
		}
	}
	//std::cout << "centerChunk: (" << centerChunk.x << ", " << centerChunk.y << ")" << std::endl;
	//std::cout << "neighbourChunks: ";
	//for (const auto& neighbour : neighbourChunks) {
	//	std::cout << "(" << neighbour.x << ", " << neighbour.y << ") ";
	//}

	// Add those chunks and generate terrain
	for (const auto& toAdd : chunksToAdd) {
		Chunk<32, 32, 32> newChunk(glm::vec2(toAdd), m_palette);
		newChunk.Generate(m_rng);
		m_chunks.emplace(toAdd, std::move(newChunk));
	}




}

//void WorldGenerator::Draw(ShaderProgram& shader) {
//	// draw all chunks
//}

void WorldGenerator::RegisterStreamingSource(IStreamingSource* source) {
	// register
	m_streamingSource = source;

}

void WorldGenerator::UnregisterStreamingSource() {
	// unregister
	m_streamingSource = nullptr;

}

Ray::HitType WorldGenerator::Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const {
	Ray::HitType hit_or_not = Ray::HitType::Miss;

	// Iterate through each chunk in the world
	for (const auto& chunkEntry : m_chunks) {
		const Chunk<32, 32, 32>& chunk = chunkEntry.second; 

		// Call the Hit function of the chunk
		Chunk<32, 32, 32>::HitRecord chunkRecord;
		Ray::HitType chunkHit = chunk.Hit(ray, min, max, chunkRecord);

		// Check if the chunk was hit and update the overall hit status
		if (chunkHit == Ray::HitType::Hit && chunkRecord.m_cubeIndex != glm::ivec3(0) ) {
			hit_or_not = Ray::HitType::Hit;

			record.m_cubeCoordinates = chunkRecord.m_cubeIndex;
			record.m_neighbourCoordinates = chunkRecord.m_neighbourIndex;
			std::cout << "Hit in chunk (" << chunkEntry.first.x << ", " << chunkEntry.first.y << ")" << std::endl;
			//std::cout << "Cube Coordinates: (" << record.m_cubeCoordinates.x << ", " << record.m_cubeCoordinates.y << ", " << record.m_cubeCoordinates.z << ")" << std::endl;
			//std::cout << "Neighbour Coordinates: (" << record.m_neighbourCoordinates.x << ", " << record.m_neighbourCoordinates.y << ", " << record.m_neighbourCoordinates.z << ")" << std::endl;

		}
	}

	return hit_or_not;
}

void WorldGenerator::RemoveBlock(const glm::ivec3& coords) {
	// Find the chunk associated with the given world coordinates
	glm::ivec2 chunkOrigin = WorldCoordsToChunkOrigin(glm::vec3(coords));
	auto it = m_chunks.find(chunkOrigin);
	constexpr uint8_t ChunkWidth = 32;
	constexpr uint8_t ChunkDepth = 32;
	if (it != m_chunks.end()) {
		// Calculate local coordinates within the chunk
		glm::ivec3 localCoords(
			coords.x - chunkOrigin.x * ChunkWidth,
			coords.y,
			coords.z - chunkOrigin.y * ChunkDepth
		);

		// Call RemoveBlock on the corresponding chunk
		it->second.RemoveBlock(localCoords.x, localCoords.y, localCoords.z);
	}
	else {
		// Handle the case when the chunk is not found
		std::cout << "Chunk not found for coordinates (" << coords.x << ", " << coords.y << ", " << coords.z << ")." << std::endl;
	}
}

void WorldGenerator::PlaceBlock(const glm::ivec3& coords, Cube::Type type) {
	constexpr uint8_t ChunkWidth = 32;  
	constexpr uint8_t ChunkDepth = 32;  

	glm::ivec2 chunkOrigin = WorldCoordsToChunkOrigin(glm::vec3(coords));
	auto it = m_chunks.find(chunkOrigin);
	if (it != m_chunks.end()) {
		glm::ivec3 localCoords(
			coords.x - chunkOrigin.x * ChunkWidth,
			coords.y,
			coords.z - chunkOrigin.y * ChunkDepth
		);
		it->second.PlaceBlock(localCoords.x, localCoords.y, localCoords.z, type);
	}
}

glm::ivec2 WorldGenerator::WorldCoordsToChunkOrigin(const glm::vec3& at) const {
	return glm::ivec2(
		static_cast<int>(std::floor(at.x / 32.0f)),
		static_cast<int>(std::floor(at.z / 32.0f))
	);
}
