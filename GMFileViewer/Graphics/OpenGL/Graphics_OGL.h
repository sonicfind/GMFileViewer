#pragma once
#include <GLFW/glfw3.h>
#include "Graphics.h"
#include <vector>
#include <mutex>


class Graphics_OGL : public Graphics
{
	class Window_OGL
	{
		GLFWwindow* m_window = nullptr;
	public:
		Window_OGL(std::string_view windowName, uint32_t width, uint32_t height);
		void Close() const;
		void endFrame() const;
		bool shouldClose() const;
		void setTitle(const char* str) const;
	};

	class Shader_OGL
	{
		// the program ID
		uint32_t ID = 0;
		static uint32_t s_activeID;

	public:
		Shader_OGL(const char* compute);
		Shader_OGL(const char* vertex, const char* fragment);
		Shader_OGL(const char* vertex, const char* geometry, const char* fragment);

		operator bool() { return ID > 0; }

		void use() const;
		void Delete() const;

		void bindUniformBlock(uint32_t bufferIndex, const char* const blockName);
		void bindStorageBlock(uint32_t bufferIndex, const char* const blockName);

		// utility uniform functions
		static void setInt(const std::string& name, int value);

	private:
		static int createShader(const char* shaderPath, GLenum shaderType);
		static void linkProgram(uint32_t id);
	};

	struct VertexBuffer
	{
		uint32_t vbo = 0;
		uint32_t vao = 0;

		void bindForUpdate() const;
		void bindForDraw() const;
		void Delete() const;
	};

	struct TextureID
	{
		std::string_view name;
		uint32_t id = 0;

		TextureID(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height);
		TextureID(TextureID&& other) noexcept;
		void bind() const;
		void Delete() const;
	};

	struct UniformBuffer
	{
		uint32_t id = 0;

		UniformBuffer() = default;
		UniformBuffer(UniformBuffer&& other) noexcept;
		void create(uint32_t index, uint32_t size);
		void bind() const;
		void Delete() const;
	};

	struct FrameBuffer
	{
		uint32_t frameID = 0;
		uint32_t colorID = 0;
		uint32_t depthID = 0;

		FrameBuffer(Graphics_OGL& gfx);
		void bindFrame() const;
		void bindColor() const;
		void Delete() const;
	};

	
public:
	Graphics_OGL(std::string name, uint32_t width, uint32_t height);
	~Graphics_OGL();

	// Inherited via Graphics
	virtual void activateShader(ShaderType type) const override;
	virtual void setShaderInt(const char* name, int value) const override;
	virtual size_t createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic) override;
	virtual void setActiveVertexBuffer(size_t index) override;
	virtual void bindVertexBuffer(size_t index) const override;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual size_t createIndexBuffer(const void* data, uint32_t dataSize) override;
	virtual void bindIndexBuffer(size_t index) const override;
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) override;
	virtual void bindTexture(std::string_view name) const override;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const override;
	virtual void unbindTexture() const override;
	virtual void selectSkyTexture(std::string_view name) override;
	virtual void setActiveTexture(uint32_t slot) const override;
	virtual void bindConstantBuffer(ConstBufferSelection selection) const override;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void setFrontFace(FrontFace front) override;
	virtual void setCullFunc(Culling cull) override;
	virtual void enable(Enablable property) const override;
	virtual void disable(Enablable property) const override;
	virtual void setDepthFunc(DepthTest testParam) const override;
	virtual void setBlendFunc(Blending blend) const override;
	virtual void setClearColor(float r, float g, float b, float a) const override;
	virtual void setTopology(PrimitiveMode mode) const override;
	virtual void drawArrays(uint32_t index, uint32_t count) const override;
	virtual void drawElements(uint32_t count, size_t offset) const override;
	virtual uint32_t addIgnorableDepthTexture() override;
	virtual void copyDepthToIgnorable(uint32_t id) const override;
	virtual void setDepthlessDepthTextures(uint32_t id) const override;
	virtual void resetFrame() const override;
	virtual void displayFrame() const override;
	virtual void updateTitle(const std::string& str) const override;
	virtual bool shouldClose() const override;

private:
	// For debug, load in the files at runtime to make testing
	// shader changes simpler to do without a rebuild
	//
	// Release will have the shaders embedded in the exe
#ifdef _DEBUG
	static const char* s_model_vert;
	static const char* s_model_frag;
	static const char* s_model_frag_depthless;
	static const char* s_sprite_vert;
	static const char* s_sprite_geo;
	static const char* s_sprite_frag;
	static const char* s_shadow_vert;
	static const char* s_shadow_geo;
	static const char* s_shadow_frag;
	static const char* s_box_vert;
	static const char* s_box_frag;
#else
	static constexpr const char* s_model_vert =
#include "ModelVertex.glsl"
		;
	static constexpr const char* s_model_frag =
#include "ModelFragment.glsl"
		;
	static constexpr const char* s_model_frag_depthless =
#include "ModelFragment_NoDepthTest.glsl"
		;
	static constexpr const char* s_sprite_vert =
#include "SpriteVertex.glsl"
		;
	static constexpr const char* s_sprite_geo =
#include "SpriteGeometry.glsl"
		;
	static constexpr const char* s_sprite_frag =
#include "SpriteFragment.glsl"
		;
	static constexpr const char* s_shadow_vert =
#include "ShadowVertex.glsl"
		;
	static constexpr const char* s_shadow_geo =
#include "ShadowGeometry.glsl"
		;
	static constexpr const char* s_shadow_frag =
#include "ShadowFragment.glsl"
		;
	static constexpr const char* s_box_vert =
#include "BoxVertex.glsl"
		;
	static constexpr const char* s_box_frag =
#include "BoxFragment.glsl"
		;
#endif

	Window_OGL m_window;
	FrameBuffer m_frame;

	Shader_OGL m_modelShader;
	Shader_OGL m_modelShader_depthless;
	Shader_OGL m_spriteShader;
	Shader_OGL m_shadowShader;
	Shader_OGL m_boxShader;

	UniformBuffer m_viewMatrix;
	UniformBuffer m_viewAndProjection;

	UniformBuffer m_modelMatrix;
	UniformBuffer m_material;
	UniformBuffer m_globalShading;
	UniformBuffer m_lights;

	static constexpr float boxVertices[] = {
		// positions // texCoords
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	std::vector<VertexBuffer> m_vertexBuffers;
	std::vector<uint32_t> m_indexBuffers;
	std::vector<TextureID> m_textures;
	const TextureID* m_skyTexture = nullptr;
	std::vector<uint32_t> ignoreIDs;

	Culling m_currentCulling = Culling::DISABLED;
	mutable GLenum m_topology = GL_TRIANGLES;
};
