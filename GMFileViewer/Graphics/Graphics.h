#pragma once
#include <memory>
#include <stdint.h>
#include <string>

enum class PrimitiveMode
{
	PRIMITIVE,
	TRIANGLE_FAN,
	TRIANGLE_STRIP,
	TRIANGLE_LIST,
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

class Graphics
{
public:
	enum class ShaderType
	{
		Base,
		Bone,
		Sprite,
		Box
	};

	virtual void activateShader(ShaderType type) const = 0;

	virtual size_t createVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic) = 0;
	virtual void bindVertexBuffer(size_t index) const = 0;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

	enum class Filtering
	{
		Bilenear,
		Nearest_Neighbor
	};
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) = 0;
	virtual void bindTexture(std::string_view name) const = 0;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const = 0;
	virtual void unbindTexture() const = 0;

	enum ConstBufferSelection
	{
		MeshMatrix,
		Material,
	};

	virtual void bindConstantBuffer(ConstBufferSelection selection) const = 0;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

	virtual void updateCameraBuffers(const float* viewMatrix, const float* projectionMatrix) const = 0;

	virtual void setCullFunc(Culling cull) = 0;
	virtual void setDepthTest(bool enable) const = 0;
	virtual void setDepthFunc(DepthTest testParam) const = 0;

	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode type) const = 0;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode type) const = 0;

	virtual void resetFrame() const = 0;
	virtual void displayFrame() const = 0;

private:
	static std::unique_ptr<Graphics> s_gfx;

public:
	enum class Backend
	{
		DirectX,
		OpenGL,
		Vulkan
	};
	static void initGraphics(Backend backend);
	static void closeGraphics();

	static Graphics* getGraphics();
};
