#pragma once
#include "Graphics.h"

class Graphics_OGL : public Graphics
{
public:

	// Inherited via Graphics
	virtual void activateShader(ShaderType type) const override;
	virtual size_t createVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic) override;
	virtual void bindVertexBuffer(size_t index) const override;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) override;
	virtual void bindTexture(std::string_view name) const override;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const override;
	virtual void unbindTexture() const override;
	virtual void bindConstantBuffer(ConstBufferSelection selection) const override;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void updateCameraBuffers(const float* viewMatrix, const float* projectionMatrix) const override;
	virtual void setCullFunc(Culling cull) override;
	virtual void setDepthTest(bool enable) const override;
	virtual void setDepthFunc(DepthTest testParam) const override;
	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode mode) const override;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode mode) const override;
	virtual void resetFrame() const override;
	virtual void displayFrame() const override;
};
