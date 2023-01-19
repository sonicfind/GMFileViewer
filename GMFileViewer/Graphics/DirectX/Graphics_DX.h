#pragma once
#include "Graphics.h"

class Graphics_DX : public Graphics
{
public:

	// Inherited via Graphics
	virtual void activateShader(ShaderType type) const override;
	virtual size_t addVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic) override;
	virtual void bindVertexBuffer(size_t index) const override;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void addTexture(const char(&name)[16], const void* imagePtr, uint32_t width, uint32_t height) override;
	virtual void bindTexture(const char(&name)[16]) const override;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const override;
	virtual void unbindTexture() const override;
	virtual void addConstantBuffer(std::string_view name, const void* data, uint32_t dataSize) override;
	virtual void connectConstantBufferToShaders(std::string_view name, ShaderType shaders[]) const override;
	virtual void bindConstantBuffer(size_t index) const override;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode type) const override;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode type) const override;
	virtual void setCullFunc(Culling cull) const override;
};
