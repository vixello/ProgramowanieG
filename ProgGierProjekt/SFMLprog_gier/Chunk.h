#pragma once
#include "Cube.h"
#include "ShaderProgram.h"
#include "PerlinNoise.h"
#include "CubePalette.h"
#include "Ray.h"
#include "AABB.h"

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>
/** Set of cubes representing fixed size chunk.
 * We associate Width dimension with X axis, Height dimension with Y axis and Depth with Z axis.
 * Order of dimensions is as follow: depth, width, heigh.
 * Each chunk has it's own 2D origin vector which represents it's world. In world coordinates
 * corresponds to first block
 */
template <uint8_t Depth, uint8_t Width, uint8_t Height>
class Chunk {
	struct CubeData {
		/** Data describing single cube in a chunk.
		 * Type of cube is used when drawing together with CubePalette to obtain OpenGL objects.
		 * Cube visibility is determined based on it's 4 closest neighbours; if all of them
		 * are different than Cube::CubeType::None we consider block to be visible.
		 */
		Cube::Type m_type{ Cube::Type::None };
		bool m_isVisible{ true };
	};
	/** Performance wise we keep all data in a flatten array.
	 * To obtain cube index based on width, heigh and depth use CoordsToIndex function
	 */
	using FlattenData_t = std::array<CubeData, Depth* Width* Height>;
public:
	/** Chunk hit record
	 * Contains all data needed after ray cast to remove and place new blocks
	 */
	struct HitRecord {
		Ray::time_t m_time;
		glm::ivec3 m_cubeIndex;
		glm::ivec3 m_neighbourIndex;
	};
	Chunk(const glm::vec2& origin, CubePalette& palette);

	void Generate(const PerlinNoise& rng);

	void Draw(ShaderProgram& shader) const;

	Ray::HitType Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const;

	bool RemoveBlock(uint8_t width, uint8_t height, uint8_t depth);
	bool PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type);
	glm::vec2 getOrigin() { return m_origin; };
private:
	size_t CoordsToIndex(size_t depth, size_t width, size_t height) const;
	void UpdateVisibility();

	CubePalette& m_palette;
	FlattenData_t m_data;
	glm::vec2 m_origin;
	AABB m_aabb;
};

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette) :
	m_origin(origin),
	m_palette(palette), 
	m_aabb(
		// Calculate chunk's AABB min point in world
		//glm::vec3(origin.x, origin.y,0.0f),
		glm::vec3(origin.x* Width, 0.0f, origin.y* Depth),
		// Calculate chunk's AABB max point in world
		//glm::vec3(origin.x + static_cast<float>(Width), static_cast<float>(Height), origin.y + static_cast<float>(Depth))
		glm::vec3((origin.x + 1)* Width, Height, (origin.y + 1)* Depth)
	) {


}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::Generate(const PerlinNoise& rng) {
	const float increment = 0.03f;

	for (int x = 0; x < Width; x++) {
		for (int z = 0; z < Depth; z++) {
			const glm::vec3 noiseCoords(
				m_origin.x * Width * increment + x * increment,
				0.12f,
				m_origin.y * Depth * increment + z * increment
			);
			const uint8_t height = static_cast<uint8_t>(rng.At(noiseCoords) * Height);
			int y = 0;
			for (; y < height - 1; y++) {
				m_data[CoordsToIndex(z, x, y)].m_type = Cube::Type::Stone;
			}
			m_data[CoordsToIndex(z, x, y)].m_type = Cube::Type::Grass;
		}
	}
	UpdateVisibility();
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::Draw(ShaderProgram& shader) const {
	glm::vec3 chunk_shift(m_origin.x * Width, 0, m_origin.y * Depth);
	glm::mat4 chunk_origin = glm::translate(glm::mat4(1.0f), chunk_shift);

	shader.Use();

	for (uint8_t height_i = 0; height_i < Height; height_i++) {
		for (uint8_t width_i = 0; width_i < Width; width_i++) {
			for (uint8_t depth_i = 0; depth_i < Depth; depth_i++) {
				const CubeData& data = m_data[CoordsToIndex(depth_i, width_i, height_i)];

				if (data.m_type != Cube::Type::None && data.m_isVisible  ) {
					const Cube& cube = m_palette.LookUp(data.m_type);

					glm::mat4 model = glm::translate(chunk_origin, glm::vec3(width_i, height_i, depth_i));
					shader.SetMat4("model", model);

					glBindTexture(GL_TEXTURE_2D, cube.Texture());
					glBindVertexArray(cube.Vao());
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
	}
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Ray::HitType Chunk<Depth, Width, Height>::Hit(const Ray& ray, Ray::time_t min, Ray::time_t max, HitRecord& record) const {
	//	define hit record for chunk's AABB
	//	early exit in case entire chunk was missed
	//	calculate chunk shift
	//	for x,y,z in Width, Height, Depth
	//		find CubeData in chunk
	//		check visibility and type
	//		define cube AABB
	//		test ray with cube
	//		if hit
	//			save last hit time to max variable
	//			save cube index to hit record
	//			calculate cube position in world (it's center!)
	//			calculate neighbour index based on axis
	//			save neighbour index to hit record
	//	if no cubes were hit return Miss
	AABB::HitRecord record_aabb;
	Ray::HitType hit_or_not = Ray::HitType::Miss;
	glm::vec3 chunk_shift(m_origin.x * Width, 0, m_origin.y * Depth);
	

	if (m_aabb.Hit(ray, min, max, record_aabb) == Ray::HitType::Miss) {
		return hit_or_not;
	}


	for (uint8_t height_i = 0; height_i < Height; height_i++) {
		for (uint8_t width_i = 0; width_i < Width; width_i++) {
			for (uint8_t depth_i = 0; depth_i < Depth; depth_i++) {
				const CubeData& data = m_data[CoordsToIndex(depth_i, width_i, height_i)];
				if (data.m_type != Cube::Type::None && data.m_isVisible) {
					
					glm::vec3 aabb_min = chunk_shift + glm::vec3(width_i, height_i, depth_i);
					glm::vec3 aabb_max = aabb_min + glm::vec3(1,1,1);

					AABB cube_AABB(aabb_min, aabb_max);
					//std::cout << "Checking cube at: " << width_i << ", " << height_i << ", " << depth_i << std::endl;
					if (cube_AABB.Hit(ray, min, max, record_aabb) == Ray::HitType::Hit) {
						max = record_aabb.m_time;
						record.m_cubeIndex = glm::vec3(width_i, height_i, depth_i);

						glm::vec3 cube_position = glm::vec3(width_i, height_i, depth_i) + 0.5f; //+0.4f
						record.m_neighbourIndex = glm::vec3(width_i, height_i, depth_i);

						size_t axis = static_cast<size_t>(record_aabb.m_axis);
						//if (record_aabb.m_point[axis] > cube_position[axis]) {
						//	record.m_neighbourIndex[axis] += 1;
						//}
						//else {
						//	record.m_neighbourIndex[axis] -= 1;
						//}
						const size_t axisIndex = static_cast<size_t>(record_aabb.m_axis);
						record.m_neighbourIndex = glm::ivec3(width_i, height_i, depth_i);
						record.m_neighbourIndex[axisIndex] += record_aabb.m_point[axisIndex] > cube_position[axisIndex] ? 1 : -1;
						record.m_time = record_aabb.m_time;
						hit_or_not = Ray::HitType::Hit;
						//std::cout << "Cube hit at: " << record.m_cubeIndex.x << ", " << record.m_cubeIndex.y << ", " << record.m_cubeIndex.z << std::endl;

					}
				}
			}
		}
	}
	return hit_or_not;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline bool Chunk<Depth, Width, Height>::RemoveBlock(uint8_t width, uint8_t height, uint8_t depth) {
	//	find CubeData in chunk
	//	check type
	//	replace type with none
	//	update visibility
	//	return operation's success
	//if (width >= Width || height >= Height || depth >= Depth) {
	//	// Coordinates are out of bounds
	//	std::cout << "Attempted to remove a block outside chunk bounds." << std::endl;
	//	return false;
	//}
	//if (width < m_aabb.Min().x || width >= m_aabb.Max().x ||
	//	height < m_aabb.Min().y || height >= m_aabb.Max().y ||
	//	depth < m_aabb.Min().z || depth >= m_aabb.Max().z) {
	//	// Coordinates are outside the chunk's AABB bounds
	//	std::cout << "Coordinates outside bounds" << std::endl;
	//	return false;
	//}
	size_t cube_index = CoordsToIndex(depth, width, height);

	if (m_data[cube_index].m_type == Cube::Type::None) {
		std::cout << "Attempted to remove an empty block." << std::endl;
		return false;
	}
	else {
		//std::cout << "Removing a block at (" << width << ", " << height << ", " << depth << ")." << std::endl;

		m_data[cube_index].m_type = Cube::Type::None;
		UpdateVisibility();
		std::cout << "Removing a block." << std::endl;

		return true;
	}
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline bool Chunk<Depth, Width, Height>::PlaceBlock(uint8_t width, uint8_t height, uint8_t depth, Cube::Type type) {
	//	find CubeData in chunk
	//	check type
	//	replace type with new type
	//	update visibility
	//	return operation's success
	// 
	//width > m_aabb.Max().x
	//	height > m_aabb.Max().y
	//	depth > m_aabb.Max().z
	   // if (width < m_aabb.Min().x || width >= m_aabb.Max().x ||
    //    height < m_aabb.Min().y || height >= m_aabb.Max().y ||
    //    depth < m_aabb.Min().z || depth >= m_aabb.Max().z) {
    //    // Coordinates are outside the chunk's AABB bounds
    //    std::cout << "Coordinates outside bounds" << std::endl;
    //    return false;
    //}

	size_t cube_index = CoordsToIndex(depth, width, height);

	if (m_data[cube_index].m_type == Cube::Type::None) {
		m_data[cube_index].m_type = type;
		UpdateVisibility();
		std::cout << "cube added" << std::endl;
		return true;
	}
	else {
		std::cout << "failed to add" << std::endl;
		auto cube = m_data[cube_index].m_type;
		std::cout << cube_index << std::endl;

		return false;
	}
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline size_t Chunk<Depth, Width, Height>::CoordsToIndex(size_t depth, size_t width, size_t height) const {
	return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
		+ width * static_cast<size_t>(Depth)
		+ depth;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::UpdateVisibility() {
	//	define 6 neighbours, one for each dimension
	//	for x, y, z in Width, Height, Depth
	//		find CubeData in chunk
	//		check type of all neighbours
	//		save visibility
	//static const std::array<glm::vec3, 3 * 3 * 3> cube_neighbours = []() {
	//	std::array<glm::vec3, 3 * 3 * 3> result;
	//	int index = 0;

	//		for (int x = -1; x < 2; x++) {
	//			for (int y = -1; y < 2; y++) {
	//				for (int z = -1; z < 2; z++) {
	//					result[index] = { x, y, z };  
	//					index++;
	//				}
	//			}
	//		}
	//		return result;
	//	} ();
		static const std::array<glm::ivec3, 6> cube_neighbours = {
		glm::ivec3{-1,  0,  0},
		glm::ivec3{ 1,  0,  0},
		glm::ivec3{ 0, -1,  0},
		glm::ivec3{ 0,  1,  0},
		glm::ivec3{ 0,  0, -1},
		glm::ivec3{ 0,  0,  1}
		};


		for (uint8_t height_i = 1; height_i < Height - 1; height_i++) {
			for (uint8_t width_i = 1; width_i < Width - 1; width_i++) {
				for (uint8_t depth_i = 1; depth_i < Depth - 1; depth_i++) {
					CubeData& current_cube = m_data[CoordsToIndex(depth_i, width_i, height_i)];
					current_cube.m_isVisible = false;

					for (const auto& cube_neighbour : cube_neighbours) {
						if (m_data[CoordsToIndex(depth_i + cube_neighbour.z, width_i + cube_neighbour.x, height_i + cube_neighbour.y)].m_type == Cube::Type::None) {
							current_cube.m_isVisible = true;
							break;
						}
					}
				}
			}
		}
	}

