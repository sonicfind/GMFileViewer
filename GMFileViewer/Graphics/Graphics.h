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

class Graphics
{
public:
	enum ShaderType
	{
		Envelope,
		Model,
		Shadow,
		Sprite,
		Sky
	};

	virtual void activateShader(ShaderType type) const = 0;
	virtual void setShaderInt(const char* name, int value) const = 0;

	virtual size_t createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic) = 0;
	virtual void bindVertexBuffer(size_t index) const = 0;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

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

	enum FrontFace
	{
		Clockwise,
		Countrclockwise,
	};
	virtual void setFrontFace(FrontFace front) const = 0;
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

	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode mode) const = 0;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode mode) const = 0;

	virtual void resetFrame() const = 0;
	virtual void displayFrame() const = 0;
	virtual void updateTitle(const std::string& str) const = 0;
	virtual bool shouldClose() const = 0;

private:
	static std::unique_ptr<Graphics> s_gfx;

	static struct GraphicsSettings
	{
		uint32_t width = 1280;
		uint32_t height = 960;
		uint32_t maxFPS = 300;
		std::string name = "Window";
	} s_settings;

public:
	enum Backend
	{
		DirectX,
		OpenGL,
		Vulkan
	};
	static void initGraphics(Backend backend);
	static void closeGraphics();

	static Graphics* getGraphics();
	virtual ~Graphics() = default;

	static uint32_t getWidth() { return s_settings.width; }
	static uint32_t getHeight() { return s_settings.height; }
	static uint32_t getMaxFPS() { return s_settings.maxFPS; }
	static std::string_view getName() { return s_settings.name; }

	static void getWidth(uint32_t width)   { s_settings.width = width; }
	static void getHeight(uint32_t height) { s_settings.height = height; }
	static void getMaxFPS(uint32_t fps)    { s_settings.maxFPS = fps; }
	static void getName(std::string name)  { s_settings.name = name; }
};
