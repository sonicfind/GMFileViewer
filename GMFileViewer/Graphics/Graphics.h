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

	virtual size_t addVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic) = 0;
	virtual void bindVertexBuffer(size_t index) const = 0;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const = 0;

	enum class Filtering
	{
		Bilenear,
		Nearest_Neighbor
	};
	virtual void addTexture(const char(&name)[16], const void* imagePtr, uint32_t width, uint32_t height) = 0;
	virtual void bindTexture(const char(&name)[16]) const = 0;
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

	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode type) const = 0;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode type) const = 0;

	enum class Culling
	{
		Disabled,
		Back,
		Front
	};
	virtual void setCullFunc(Culling cull) const = 0;

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
