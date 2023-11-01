#pragma once
#include <memory>
#include <stdint.h>
#include <string>

enum class PrimitiveMode
{
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	PRIMITIVE,
};

enum class Culling : uint32_t
{
	DISABLED,
	BACK,
	FRONT
};

enum class FrontFace : uint32_t
{
	Countrclockwise,
	Clockwise,
};

enum class DepthTest : uint32_t
{
	ALWAYS,
	NEVER,
	LESS,
	EQUAL,
	L_EQUAL,
	GREATER,
	NOT_EQUAL,
	G_EQUAL
};

enum class Blending : uint32_t
{
	Opaque,
	Additive,
	Multipy,
	Subtract,
	Unknown,
	MaterialAlpha
};

enum class Backend
{
	DirectX,
	OpenGL,
	Vulkan
};

class Graphics
{
	struct GraphicsSettings
	{
		std::string name = "Window";
		uint32_t width = 1280;
		uint32_t height = 960;
		uint32_t maxFPS = 300;
	};

protected:
	GraphicsSettings m_settings;
	Graphics(std::string window, uint32_t width, uint32_t height, uint32_t maxFPS);

public:
	uint32_t getWidth() const { return m_settings.width; }
	uint32_t getHeight() const { return m_settings.height; }
	uint32_t getMaxFPS() const { return m_settings.maxFPS; }
	std::string_view getName() const { return m_settings.name; }

	void setWidth(uint32_t width) { m_settings.width = width; }
	void setHeight(uint32_t height) { m_settings.height = height; }
	void setMaxFPS(uint32_t fps) { m_settings.maxFPS = fps; }
	void setName(std::string name) { m_settings.name = name; }

public:
	enum ShaderType
	{
		Envelope,
		Model,
		Model_NoDepth,
		Shadow,
		Sprite,
		Box
	};

	virtual void activateShader(ShaderType type) const = 0;
	virtual void setShaderInt(const char* name, int value) const = 0;

	virtual size_t createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic) = 0;
	virtual void setActiveVertexBuffer(size_t index) = 0;
	virtual void bindVertexBuffer(size_t index) const = 0;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

	virtual size_t createIndexBuffer(const void* data, uint32_t dataSize) = 0;
	virtual void bindIndexBuffer(size_t index) const = 0;

	enum Filtering
	{
		Bilenear,
		Nearest_Neighbor
	};
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) = 0;
	virtual void bindTexture(std::string_view name) const = 0;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const = 0;
	virtual void unbindTexture() const = 0;
	virtual void selectSkyTexture(std::string_view name) = 0;
	virtual void setActiveTexture(uint32_t slot) const = 0;

	enum ConstBufferSelection
	{

		View,
		ComboViewAndProjection,
		ModelMatrix,
		Material,
		GlobalShading,
		Lights
	};

	virtual void bindConstantBuffer(ConstBufferSelection selection) const = 0;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

	
	virtual void setFrontFace(FrontFace front) = 0;
	virtual void setCullFunc(Culling cull) = 0;
	virtual void setDepthFunc(DepthTest testParam) const = 0;

	virtual void setBlendFunc(Blending blend) const = 0;
	virtual void setClearColor(float r, float g, float b, float a) const = 0;

	enum Enablable
	{
		Depth_Test,
		Depth_Mask,
		AlphaBlending
	};
	virtual void enable(Enablable property) const = 0;
	virtual void disable(Enablable property) const = 0;

	virtual void setTopology(PrimitiveMode mode) const = 0;
	virtual void drawArrays(uint32_t index, uint32_t count) const = 0;
	virtual void drawElements(uint32_t count, size_t offset) const = 0;
	/*virtual void getDepthData(float* buffer) const = 0;
	virtual void copyDefaultDepthData() const = 0;
	virtual void compareAndSetDepthData(float* buffer) const = 0;
	virtual void setDefaultDepthData() const = 0;*/

	virtual void resetFrame() const = 0;
	virtual void displayFrame() const = 0;
	virtual void updateTitle(const std::string& str) const = 0;
	virtual bool shouldClose() const = 0;

	virtual uint32_t addIgnorableDepthTexture() = 0;
	virtual void copyDepthToIgnorable(uint32_t id)  const = 0;
	virtual void setDepthlessDepthTextures(uint32_t id)  const = 0;
};
