
#include "WorldGenerator.h"
#include "Player.h"
#include "MinecraftClone.h"
#include "Serialization.h"

#include <cmath>
#include <algorithm>
#include <iterator>

WorldGenerator::WorldGenerator(MinecraftClone& instance, size_t renderDistance)
	: m_renderDistance(renderDistance)
	, m_instance(instance) {
	
	m_worldDirectory = std::filesystem::current_path().string();
	m_worldDirectory = (std::filesystem::path(m_worldDirectory) / "world_data").string();
	std::filesystem::create_directories(m_worldDirectory);

	// register renderable
	m_instance.GetRenderer()->RegisterRenderable(this);
}

WorldGenerator::~WorldGenerator() {
	// unregister renderable
	m_instance.GetRenderer()->UnregisterRenderable(this);

}

void WorldGenerator::Update(float deltaTime) {
	//// Update all entities
	for (auto& entity : m_entities) {
		entity->Update(deltaTime);
	}

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
	//if (m_streamingSource) {
	//	static_cast<CameraStreamingSource*>(m_streamingSource)->UpdatePosition();
	//}
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

	for (auto& chunkOrigin : chunksToRemove) {
		FixedSizeChunk& chunk = m_chunks.at(chunkOrigin);
		if (chunk.NeedsSaving()) {
			std::filesystem::path chunkPath = m_worldDirectory;
			chunkPath.append("chunk." + std::to_string(chunkOrigin.x) + "." + std::to_string(chunkOrigin.y) + ".json");
			std::ofstream chunkFile(chunkPath);

			const nlohmann::json chunkData = std::move(chunk); // please see Serialization.h comment about move-only types

			chunkFile << chunkData;
		}
		m_chunks.erase(chunkOrigin);

	}

	// Erase those chunks
	for (const auto& toRemove : chunksToRemove) {
		m_chunks.erase(toRemove);
	}

	//// ChunksToAdd is a set_difference between neighbours chunks and current chunks
	std::vector<glm::ivec2> chunksToAdd;
	std::set_difference(neighbourChunks.begin(), neighbourChunks.end(),
		chunkArray.begin(), chunkArray.end(),
		std::back_inserter(chunksToAdd), comp);
// Chunks to add is a set_difference between neighbours chunk and current chunks
	//std::vector<glm::ivec2> chunksToAdd;

	//for (const auto& neighbour : neighbourChunks) {
	//	auto it = std::find(chunkArray.begin(), chunkArray.end(), neighbour);
	//	if (it == chunkArray.end()) {
	//		chunksToAdd.push_back(neighbour);
	//	}
	//}
	//std::cout << "centerChunk: (" << centerChunk.x << ", " << centerChunk.y << ")" << std::endl;
	//std::cout << "neighbourChunks: ";
	//for (const auto& neighbour : neighbourChunks) {
	//	std::cout << "(" << neighbour.x << ", " << neighbour.y << ") ";
	//}

	// Add those chunks and generate terrain
	//for (const auto& toAdd : chunksToAdd) {
	//	FixedSizeChunk newChunk(glm::vec2(toAdd), m_palette);
	//	newChunk.Generate(m_rng);
	//	m_chunks.emplace(toAdd, std::move(newChunk));
	//}
	for (auto& chunkOrigin : chunksToAdd) {
		std::filesystem::path chunkPath = m_worldDirectory;
		chunkPath.append("chunk." + std::to_string(chunkOrigin.x) + "." + std::to_string(chunkOrigin.y) + ".json");
		if (std::filesystem::exists(chunkPath)) {
			try {
				std::cout << "FILE EXISTS" << std::endl;
			std::ifstream chunkFile(chunkPath);
			std::cout << "chunkFile" << std::endl;

			//nlohmann::json chunkData = nlohmann::json::parse(chunkFile);
			//this desnt work 
			//m_chunks.emplace(chunkOrigin, std::move(chunkData.get<FixedSizeChunk>()));
			
			//if (chunkFile.peek() != std::ifstream::traits_type::eof()) {
				// Check if the file is not empty before attempting to parse
				//nlohmann::json chunkData = nlohmann::json::parse(chunkFile);
				//m_chunks.emplace(chunkOrigin, std::move(chunkData.get<FixedSizeChunk>()));
				//chunkFile >> chunkData;

				// Deserialize the chunk and add it to m_chunks
				//FixedSizeChunk loadedChunk = chunkData;
				//m_chunks.emplace(chunkOrigin, std::move(loadedChunk));
				//nlohmann::json chunkData;
				//chunkFile >> chunkData;
				nlohmann::json chunkData = nlohmann::json::parse(chunkFile);
				std::cout << "chunkData" << std::endl;
				//FixedSizeChunk newChunk = chunkData.template get<FixedSizeChunk>();

				//chunkData.template get<FixedSizeChunk>().Generate(m_rng);
				//std::cout << "Generate" << std::endl;

				//FixedSizeChunk newChunk = chunkData.get<FixedSizeChunk>();
				//m_chunks.emplace(chunkOrigin, std::move(newChunk));
				//m_chunks.emplace(chunkOrigin, std::move(chunkData.template get<FixedSizeChunk>()));
				std::cout << "emplace" << std::endl;

				// 
			//}

			//m_chunks.emplace(chunkOrigin, std::move(newChunk));
			}
			catch (const std::exception& e) {
				// Handle parsing errors
				std::cout << "Error parsing chunk JSON: " << e.what() << std::endl;
			}

		}
		else {
			FixedSizeChunk newChunk(glm::vec2(chunkOrigin), m_palette);
			newChunk.Generate(m_rng);
			m_chunks.emplace(chunkOrigin, std::move(newChunk));
			//m_chunks.try_emplace(chunkOrigin, chunkOrigin, m_rng);
		}
	}
	//for (auto& chunkOrigin : chunksToAdd){
	//	const auto it = [&]()-> decltype(auto) {
	//		std::filesystem::path chunkPath = m_worldDirectory;
	//		chunkPath.append("chunk." + std::to_string(chunkOrigin.x) + "." + std::to_string(chunkOrigin.y) + ".json");
	//		if (std::filesystem::exists(chunkPath)) {
	//			std::ifstream chunkFile(chunkPath);
	//			nlohmann::json chunkData = nlohmann::json::parse(chunkFile);
	//			return m_chunks.try_emplace(chunkOrigin, std::move(chunkData.template get<FixedSizeChunk>()));
	//		}
	//		else {
	//			//return m_chunks.try_emplace(chunkOrigin, chunkOrigin, m_rng);
	//			FixedSizeChunk newChunk(glm::vec2(chunkOrigin), m_palette);
	//			newChunk.Generate(m_rng);
	//			m_chunks.emplace(chunkOrigin, std::move(newChunk));
	//		};
	//	}();
	//	if (it.second) {
	//		m_instance.GetRenderer()->RegisterRenderable(&it.first->second);
	//	}
	//}


}

void WorldGenerator::Draw(ShaderProgram& shader) {
	// draw all chunks
	for (const auto& chunkEntry : m_chunks) {
		const Chunk<32, 32, 32>& chunk = chunkEntry.second;

		// Draw function of each chunk
		chunk.Draw(shader);
	}
}

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
	constexpr uint8_t ChunkWidth = 32;
	constexpr uint8_t ChunkDepth = 32;
	// Iterate through each chunk in the world
	for (const auto& chunkEntry : m_chunks) {
		const Chunk<32, 32, 32>& chunk = chunkEntry.second; 

		Chunk<32, 32, 32>::HitRecord chunkRecord;
		Ray::HitType chunkHit = chunk.Hit(ray, min, max, chunkRecord);

		// Check if the chunk was hit 
		if (chunkHit == Ray::HitType::Hit && chunkRecord.m_cubeIndex != glm::ivec3(0) ) {
			hit_or_not = Ray::HitType::Hit;

			glm::ivec3 localCoords = chunkRecord.m_cubeIndex + glm::ivec3(chunkEntry.first.x * ChunkWidth, 0, chunkEntry.first.y * ChunkDepth);
			std::cout << "localCoords (" << chunkEntry.first.x * ChunkWidth << ", " << chunkEntry.first.y * ChunkDepth << ")" << std::endl;

			record.m_cubeCoordinates = localCoords;
			//record.m_cubeCoordinates = chunkRecord.m_cubeIndex;
			record.m_neighbourCoordinates = chunkRecord.m_neighbourIndex + glm::ivec3(chunkEntry.first.x * ChunkWidth, 0, chunkEntry.first.y * ChunkDepth);
			record.m_chunkCoordinates = chunkEntry.first;

			std::cout << "Hit in chunk (" << chunkEntry.first.x << ", " << chunkEntry.first.y << ")" << std::endl;
			//std::cout << "m_cubeCoordinates (" << record.m_cubeCoordinates.x << ", " << record.m_cubeCoordinates.y << ", " << record.m_cubeCoordinates.z << ")" << std::endl;
			//std::cout << "Cube hit Coordinates: (" << record.m_cubeCoordinates.x << ", " << record.m_cubeCoordinates.y << ", " << record.m_cubeCoordinates.z << ")" << std::endl;
			//std::cout << "Neighbour Coordinates: (" << record.m_neighbourCoordinates.x << ", " << record.m_neighbourCoordinates.y << ", " << record.m_neighbourCoordinates.z << ")" << std::endl;
			//std::cout << "Chunk Coordinates (" << WorldCoordsToChunkOrigin(glm::vec3(record.m_cubeCoordinates)).x << ", " << WorldCoordsToChunkOrigin(glm::vec3(record.m_cubeCoordinates)).y << ")" << std::endl;
			//std::cout << "World Coordinates (" << ray.Origin().x << ", " << ray.Origin().y << ", " << ray.Origin().z << ")" << std::endl;

		}
	}

	return hit_or_not;
}

void WorldGenerator::RemoveBlock(const glm::ivec3& coords) {
	// Find the chunk based on given world coordinates
	glm::ivec2 chunkOrigin = WorldCoordsToChunkOrigin(glm::vec3(coords));
	std::cout << "Chunk Coordinates: (" << chunkOrigin.x << ", " << chunkOrigin.y << ")" << std::endl;
	std::cout << "Cube Coordinates: (" << coords.x << ", " << coords.y << ", " << coords.z << ")" << std::endl;

	std::cout << std::endl;
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
		std::cout << "Chunk found for coordinates (" << coords.x << ", " << coords.y << ", " << coords.z
			<< ") in chunk (" << chunkOrigin.x << ", " << chunkOrigin.y << ")." << std::endl;
		std::cout << "Local chunk Coordinates: (" << localCoords.x << ", " << localCoords.y << ", " << localCoords.z << ")" << std::endl;

		it->second.RemoveBlock(localCoords.x, localCoords.y, localCoords.z);
	}
	else {
		// Handle the case when the chunk is not found
		std::cout << "Chunk not found for coordinates (" << coords.x << ", " << coords.y << ", " << coords.z << ")." << std::endl;
		std::cout << "Player position: " << m_streamingSource->GetPosition().x << ", "
			<< m_streamingSource->GetPosition().y << ", "
			<< m_streamingSource->GetPosition().z << std::endl;

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
