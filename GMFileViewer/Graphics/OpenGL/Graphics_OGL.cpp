#include <GLAD/glad.h>
#include "Graphics_OGL.h"
#include <map>

#ifdef _DEBUG
const char* Graphics_OGL::s_model_vert = "Graphics/OpenGL/ModelVertex.glsl";
const char* Graphics_OGL::s_model_frag = "Graphics/OpenGL/ModelFragment.glsl";
const char* Graphics_OGL::s_sprite_vert = "Graphics/OpenGL/SpriteVertex.glsl";
const char* Graphics_OGL::s_sprite_geo = "Graphics/OpenGL/SpriteGeometry.glsl";
const char* Graphics_OGL::s_sprite_frag = "Graphics/OpenGL/SpriteFragment.glsl";
const char* Graphics_OGL::s_shadow_vert = "Graphics/OpenGL/ShadowVertex.glsl";
const char* Graphics_OGL::s_shadow_geo = "Graphics/OpenGL/ShadowGeometry.glsl";
const char* Graphics_OGL::s_shadow_frag = "Graphics/OpenGL/ShadowFragment.glsl";
const char* Graphics_OGL::s_sky_vert = "Graphics/OpenGL/SkyVertex.glsl";
const char* Graphics_OGL::s_sky_frag = "Graphics/OpenGL/SkyFragment.glsl";

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    static std::map<GLenum, const GLchar*> Sources =
    {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
        {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
        {GL_DEBUG_SOURCE_OTHER, "OTHER"}
    };

    static std::map<GLenum, const GLchar*> Severities =
    {
        {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW, "LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
    };

    static std::map<GLenum, const GLchar*> Types =
    {
        {GL_DEBUG_TYPE_ERROR, "ERROR"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
        {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
        {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
        {GL_DEBUG_TYPE_MARKER, "MARKER"},
        {GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
        {GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
        {GL_DEBUG_TYPE_OTHER, "OTHER"}
    };

    printf("[OpenGL %s] - SEVERITY: %s, SOURCE: %s, ID: %d: %s\n", Types[type], Severities[severity], Sources[source], id, message);
}
#endif

Graphics_OGL::Graphics_OGL()
    : m_window(Graphics::getName(), Graphics::getWidth(), Graphics::getHeight())
    , m_modelShader(s_model_vert, s_model_frag)
    , m_shadowShader(s_shadow_vert, s_shadow_geo, s_shadow_frag)
    , m_spriteShader(s_sprite_vert, s_sprite_geo, s_sprite_frag)
    , m_skyShader(s_sky_vert, s_sky_frag)
{
    auto const bindAll = [&](uint32_t index, const char* const blockName)
    {
        m_modelShader.bindUniformBlock(index, blockName);
        m_spriteShader.bindUniformBlock(index, blockName);
        m_shadowShader.bindUniformBlock(index, blockName);
    };

    m_viewMatrix.create(0, 64);
    m_modelShader.bindUniformBlock(0, "View");
    m_spriteShader.bindUniformBlock(0, "View");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_viewMatrix.id);

    m_viewAndProjection.create(1, 64);
    m_modelShader.bindUniformBlock(1, "Combo");
    m_spriteShader.bindUniformBlock(1, "Combo");
    m_shadowShader.bindUniformBlock(1, "Combo");
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_viewAndProjection.id);

    m_modelMatrix.create(2, 64);
    m_modelShader.bindUniformBlock(2, "Model");
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_modelMatrix.id);

    m_material.create(3, 64);
    m_modelShader.bindUniformBlock(3, "Material");
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_material.id);

    m_lights.create(4, 304);
    m_modelShader.bindUniformBlock(4, "Lights");
    glBindBufferBase(GL_UNIFORM_BUFFER, 4, m_lights.id);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Graphics_OGL::Window_OGL::Window_OGL(std::string_view windowName, uint32_t width, uint32_t height)
{
    glfwInit();
    // Version 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, windowName.data(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw "Failed to create GLFW window";
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw "Failed to initialize GLAD";

    glViewport(0, 0, width, height);

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif
}

Graphics_OGL::Window_OGL::~Window_OGL()
{
    glfwTerminate();
}

void Graphics_OGL::activateShader(ShaderType type) const
{
    switch (type)
    {
    case Graphics::Envelope:
        throw "Unused shader";
        break;
    case Graphics::Model:
        m_modelShader.use();
        break;
    case Graphics::Shadow:
        m_shadowShader.use();
        break;
    case Graphics::Sprite:
        m_spriteShader.use();
        break;
    case Graphics::Sky:
        m_skyShader.use();
        break;
    default:
        break;
    }
}

void Graphics_OGL::setShaderInt(const char* name, int value) const
{
    Shader_OGL::setInt(name, value);
}

size_t Graphics_OGL::createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic)
{
    m_vertexBuffers.emplace_back(type, data, dataSize, isDynamic);
    return m_vertexBuffers.size() - 1;
}

void Graphics_OGL::bindVertexBuffer(size_t index) const
{
    m_vertexBuffers[index].bind();
}

void Graphics_OGL::updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

void Graphics_OGL::createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height)
{
    m_textures.emplace_back(name, imagePtr, width, height);
    unbindTexture();
}

void Graphics_OGL::bindTexture(std::string_view name) const
{
    for (const auto& texture : m_textures)
        if (texture.name == name)
        {
            glBindTexture(GL_TEXTURE_2D, texture.id);
            break;
        }
}

void Graphics_OGL::updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, locationX, locationY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imagePtr);
}

void Graphics_OGL::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics_OGL::selectSkyTexture(std::string_view name)
{
    for (const auto& texture : m_textures)
        if (texture.name == name)
        {
            m_skyTexture = &texture;
            break;
        }
}

void Graphics_OGL::setActiveTexture(uint32_t slot) const
{
    if (slot < 32)
        glActiveTexture(GL_TEXTURE0 + slot);
}

void Graphics_OGL::bindConstantBuffer(ConstBufferSelection selection) const
{
    switch (selection)
    {
    case Graphics::View:
        m_viewMatrix.bind();
        break;
    case Graphics::ComboViewAndProjection:
        m_viewAndProjection.bind();
        break;
    case Graphics::ModelMatrix:
        m_modelMatrix.bind();
        break;
    case Graphics::Material:
        m_material.bind();
        break;
    case Graphics::Lights:
        m_lights.bind();
        break;
    default:
        break;
    }
}

void Graphics_OGL::updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
}

void Graphics_OGL::setFrontFace(FrontFace front) const
{
    if (front == Clockwise)
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
}

void Graphics_OGL::setCullFunc(Culling cull)
{
    if (m_currentCulling != cull)
    {
        switch (cull)
        {
        case Culling::DISABLED:
            glDisable(GL_CULL_FACE);
            break;
        case Culling::BACK:
            if (m_currentCulling == Culling::DISABLED)
                glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case Culling::FRONT:
            if (m_currentCulling == Culling::DISABLED)
                glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        default:
            return;
        }
        m_currentCulling = cull;
    }
}

void Graphics_OGL::enable(Enablable property) const
{
    switch (property)
    {
    case Graphics::Depth_Test:
        glEnable(GL_DEPTH_TEST);
        break;
    case Graphics::Depth_Mask:
        glDepthMask(GL_TRUE);
        break;
    case Graphics::AlphaBlending:
        glEnable(GL_BLEND);
        break;
    default:
        break;
    }
}

void Graphics_OGL::disable(Enablable property) const
{
    switch (property)
    {
    case Graphics::Depth_Test:
        glDisable(GL_DEPTH_TEST);
        break;
    case Graphics::Depth_Mask:
        glDepthMask(GL_FALSE);
        break;
    case Graphics::AlphaBlending:
        glDisable(GL_BLEND);
        break;
    default:
        break;
    }
}

void Graphics_OGL::setDepthFunc(DepthTest testParam) const
{
    switch (testParam)
    {
    case DepthTest::ALWAYS:
        glDepthFunc(GL_ALWAYS);
        break;
    case DepthTest::NEVER:
        glDepthFunc(GL_NEVER);
        break;
    case DepthTest::LESS:
        glDepthFunc(GL_LESS);
        break;
    case DepthTest::EQUAL:
        glDepthFunc(GL_EQUAL);
        break;
    case DepthTest::L_EQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    case DepthTest::GREATER:
        glDepthFunc(GL_GREATER);
        break;
    case DepthTest::NOT_EQUAL:
        glDepthFunc(GL_NOTEQUAL);
        break;
    case DepthTest::G_EQUAL:
        glDepthFunc(GL_GEQUAL);
        break;
    default:
        break;
    }
}

void Graphics_OGL::setBlendFunc(Blending blend) const
{
    switch (blend)
    {
    case Blending::Additive:
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);
        break;
    case Blending::Multipy:
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        break;
    case Blending::Subtract:
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
        break;
    default:
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
        break;
    }
}

void Graphics_OGL::setClearColor(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
}

void Graphics_OGL::drawArrays(uint32_t index, uint32_t count, PrimitiveMode mode) const
{
    glDrawArrays(GL_TRIANGLES + static_cast<uint32_t>(mode), index, count);
}

void Graphics_OGL::drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode mode) const
{
    glDrawElements(GL_TRIANGLES + static_cast<uint32_t>(mode), count, GL_UNSIGNED_INT, indices);
}

void Graphics_OGL::resetFrame() const
{
    glDepthMask(GL_TRUE);
    if (!m_skyTexture)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics_OGL::displayFrame() const
{
    m_window.endFrame();
}

void Graphics_OGL::updateTitle(const std::string& str) const
{
    m_window.setTitle(str.c_str());
}

bool Graphics_OGL::shouldClose() const
{
    return m_window.shouldClose();
}

void Graphics_OGL::Window_OGL::endFrame() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Graphics_OGL::Window_OGL::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Graphics_OGL::Window_OGL::setTitle(const char* str) const
{
    glfwSetWindowTitle(m_window, str);
}

Graphics_OGL::TextureID::TextureID(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) : name(name)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagePtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Graphics_OGL::TextureID::TextureID(TextureID&& other) noexcept : id(other.id), name(other.name)
{
    other.id = 0;
}

void Graphics_OGL::TextureID::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

Graphics_OGL::TextureID::~TextureID()
{
    glDeleteTextures(1, &id);
}

Graphics_OGL::VertexBuffer::VertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic)
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    switch (type)
    {
    case Graphics::Model:
    {
        static constexpr size_t VERTSIZE_MODEL = 52;
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VERTSIZE_MODEL, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTSIZE_MODEL, (void*)(4 * sizeof(float)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTSIZE_MODEL, (void*)(7 * sizeof(float)));
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTSIZE_MODEL, (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
    }
    break;
    case Graphics::Shadow:
    {
        static constexpr size_t VERTSIZE_SHADOW = 64;
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VERTSIZE_SHADOW, (void*)0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTSIZE_SHADOW, (void*)(4 * sizeof(float)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTSIZE_SHADOW, (void*)(8 * sizeof(float)));
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, VERTSIZE_SHADOW, (void*)(12 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
    }
    break;
    case Graphics::Sprite:
    {

    }
    break;
    case Graphics::Sky:
    {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
    break;
    default:
        break;
    }

    glBufferData(GL_ARRAY_BUFFER, dataSize, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

Graphics_OGL::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : vao(other.vao), vbo(other.vbo)
{
    other.vao = 0;
    other.vbo = 0;
}

void Graphics_OGL::VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
}

Graphics_OGL::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

Graphics_OGL::UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : id(other.id)
{
    other.id = 0;
}

void Graphics_OGL::UniformBuffer::create(uint32_t index, uint32_t size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

void Graphics_OGL::UniformBuffer::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
}

Graphics_OGL::UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &id);
}
