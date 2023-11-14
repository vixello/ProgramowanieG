#include "Cube.h"

#include <SFML/Graphics.hpp>


void Cube::CreateVertexBufferObject() {

	//float vertices[] = {
	//	 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	//	 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
	//	-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	//};
	//GLuint m_vbo, m_vao;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices.data(), GL_STATIC_DRAW);

	// 3 floaty x y z reprezentuj¹ce pozycje w œwiecie gry
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 2 floaty u v reprezentuj¹ce miejsce na teksturze
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//return std::make_pair(m_vbo, m_vao);
};
std::array<float, 6 * 6 * 5> Cube::s_vertices = {
       //x       y     z       u     v
		//back
        -0.5f, -0.5f, -0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.25f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
		 0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
		-0.5f,  0.5f, -0.5f,  0.5f, 1.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  0.5f, 0.0f,

		//front
        -0.5f, -0.5f,  0.5f,  0.25f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.5f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
		 0.5f,  0.5f,  0.5f,  0.5f, 2.0f / 3.0f,
		-0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,
		-0.5f, -0.5f,  0.5f,  0.25f, 1.f,

		//left
        -0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,
		-0.5f,  0.5f, -0.5f,  0.25f, 1.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f / 3.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 2.0f / 3.0f,
		-0.5f,  0.5f,  0.5f,  0.25f, 2.0f / 3.0f,

		//right
         0.5f,  0.5f,  0.5f,  0.5f, 1.0f / 3.0f,
		 0.5f,  0.5f, -0.5f,  0.5f, 2.0f / 3.0f,
		 0.5f, -0.5f, -0.5f,  0.75f, 2.0f / 3.0f,
		 0.5f, -0.5f, -0.5f,  0.75f, 2.0f / 3.0f,
		 0.5f, -0.5f,  0.5f,  0.75f, 1.0f / 3.0f,
		 0.5f,  0.5f,  0.5f,  0.5f, 1.0f / 3.0f,

		 //down
        -0.5f, -0.5f, -0.5f,  0.75f, 2.0f / 3.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 2.0f / 3.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f / 3.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f / 3.0f,
		-0.5f, -0.5f,  0.5f,  0.75f, 1.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  0.75f, 2.0f / 3.0f,

		//up
        -0.5f,  0.5f, -0.5f,  0.25f, 2.0f / 3.0f,
         0.5f,  0.5f, -0.5f,  0.5f, 2.0f / 3.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 1.0f / 3.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 1.0f / 3.0f,
        -0.5f,  0.5f,  0.5f,  0.25f, 1.0f / 3.0f,
        -0.5f,  0.5f, -0.5f,  0.25f, 2.0f / 3.0f,
};

Cube::Cube(Cube&& rhs) noexcept
	: m_vbo(std::exchange(rhs.m_vbo, 0))
	, m_vao(std::exchange(rhs.m_vao, 0))
	, m_texture(std::exchange(rhs.m_texture, 0)) {
}
Cube::Cube(const std::string& texturePath) {
	m_texture = CreateTexture(texturePath);
	CreateVertexBufferObject();

}

Cube::~Cube(){
}

Cube& Cube::operator=(Cube&& rhs) noexcept {
	if (&rhs == this) {
		return *this;
	}

	m_vbo = std::exchange(rhs.m_vbo, 0);
	m_vao = std::exchange(rhs.m_vao, 0);
	m_texture = std::exchange(rhs.m_texture, 0);

	return *this;
}

GLuint Cube::CreateTexture(const std::string& path) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	sf::Image image;
	if (image.loadFromFile(path)) {
		image.flipVertically();
		const sf::Vector2u size = image.getSize();
		std::cout << "Loaded texture: " << path << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
		return 0;  // Texture loading failed
	}

	return texture;
}