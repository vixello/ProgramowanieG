#pragma once
#include "Cube.h"
#include "ShaderProgram.h"
#include "PerlinNoise.h"
#include "CubePalette.h"

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

template <uint8_t Depth, uint8_t Width, uint8_t Height>
class Chunk {
	struct CubeData {
		Cube::Type m_type{ Cube::Type::None };
		bool m_isVisible{ true };
	};

	using FlattenData_t = std::array<CubeData, Depth* Width* Height>;
public:
	Chunk(const glm::vec2& origin, CubePalette& palette);

	void Generate(const PerlinNoise& rng);

	void Draw(ShaderProgram& shader) const;

private:
	size_t CoordsToIndex(size_t depth, size_t width, size_t height) const;
	void UpdateVisibility();

	CubePalette& m_palette;
	FlattenData_t m_data;
	glm::vec2 m_origin;
};

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline Chunk<Depth, Width, Height>::Chunk(const glm::vec2& origin, CubePalette& palette) :
	m_origin(origin),
	m_palette(palette) {

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
inline size_t Chunk<Depth, Width, Height>::CoordsToIndex(size_t depth, size_t width, size_t height) const {
	return height * static_cast<size_t>(Depth) * static_cast<size_t>(Width)
		+ width * static_cast<size_t>(Depth)
		+ depth;
}

template<uint8_t Depth, uint8_t Width, uint8_t Height>
inline void Chunk<Depth, Width, Height>::UpdateVisibility() {
	static const std::array<glm::vec3, 3 * 3 * 3> cube_neighbours = []() {
		std::array<glm::vec3, 3 * 3 * 3> result;
		int index = 0;

			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					for (int z = -1; z < 2; z++) {
						result[index] = { x, y, z };  
						index++;
					}
				}
			}
			return result;
		} ();


		for (uint8_t height_i = 1; height_i < Height - 1; height_i++) {
			for (uint8_t width_i = 1; width_i < Width - 1; width_i++) {
				for (uint8_t depth_i = 1; depth_i < Depth - 1; depth_i++) {
					CubeData& current_cube = m_data[CoordsToIndex(depth_i, width_i, height_i)];
					current_cube.m_isVisible = false;


					for (const auto& cube_neighbour : cube_neighbours) {
						size_t neighbor_index = CoordsToIndex(
							depth_i + static_cast<size_t>(cube_neighbour.z),
							width_i + static_cast<size_t>(cube_neighbour.x),
							height_i + static_cast<size_t>(cube_neighbour.y)
						);

						if (m_data[neighbor_index].m_type != Cube::Type::None) {
							current_cube.m_isVisible = true;
							break;
						}
					}


				}
			}
		}
	}

