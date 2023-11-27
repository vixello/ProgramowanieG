#pragma once
#include <glad/glad.h>

#include <string>
#include <array>
#include <iostream>
class Cube {
public:
	enum class Type {
		None,
		Grass,
		Stone
	};

	Cube(const std::string& texturePath);

	Cube() = delete;
	Cube(const Cube&) = delete;
	Cube& operator=(const Cube&) = delete;
	Cube(Cube&&) noexcept;
	Cube& operator=(Cube&&) noexcept;
	~Cube();

	GLuint Vbo() const { return m_vbo; }
	GLuint Vao() const { return m_vao; }
	GLuint Texture() const { return m_texture; }
	void CreateVertexBufferObject(std::string);
private:
	GLuint m_vbo{ 0 };
	GLuint m_vao{ 0 };
	GLuint m_ebo{ 0 };
	GLuint m_texture{ 0 };
	Type m_type;

	GLuint CreateTexture(const std::string& path);

	static std::array<float, 6 * 6 * 5> s_vertices;
};