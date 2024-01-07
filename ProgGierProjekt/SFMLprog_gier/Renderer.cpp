#include "Renderer.h"


#include <algorithm>

Renderer::Renderer() {
    // load OpenGL functions
    sf::ContextSettings contextSettings;

    bool sRgb = false;
    bool exit = false;

    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = sRgb;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;

    gladLoadGL();

    // set viewport
    glViewport(0, 0, 1200,800);
    //    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    // enable GL_DEPTH_TEST
    glEnable(GL_DEPTH_TEST);

    m_shader = std::make_unique<ShaderProgram>();
}

void Renderer::Draw() {
    // clear background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // clear GL_COLOR_BUFFER_BIT and GL_DEPTH_BUFFER_BIT
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set shader as active
    m_shader->Use();
    // pass projection * view to shader
    const glm::mat4& projectionView = m_camera->Projection() * m_camera->View();
    m_shader->SetMat4("projectionView", projectionView);

    // call all renderables with shader
    for (const auto& renderable : m_renderables) {
        renderable->Draw(*m_shader);
    }
}

void Renderer::RegisterCamera(Camera* camera) {
    m_camera = camera;
}

void Renderer::UnregisterCamera(Camera* camera) {
    if (m_camera == camera) {
        m_camera = nullptr;
    }
}

void Renderer::RegisterRenderable(IRenderable* renderable) {
    // push back unique
    auto it = std::find(m_renderables.begin(), m_renderables.end(), renderable);
    if (it == m_renderables.end()) {
        m_renderables.push_back(renderable);
    }
}

void Renderer::UnregisterRenderable(IRenderable* renderable) {
    // erase-by-remove idiom
    m_renderables.erase(std::remove(m_renderables.begin(), m_renderables.end(), renderable), m_renderables.end());

}
