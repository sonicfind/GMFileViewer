#pragma once
#include "Graphics.h"

class Graphics_DX : public Graphics
{
public:

	// Inherited via Graphics
	virtual void activateShader(ShaderType type) const override;
	virtual void setShaderInt(const char* name, int value) const override;
	virtual size_t createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic) override;
	virtual void bindVertexBuffer(size_t index) const override;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) override;
	virtual void bindTexture(std::string_view name) const override;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const override;
	virtual void unbindTexture() const override;
	virtual void selectSkyTexture(std::string_view name) override;
	virtual void setActiveTexture(uint32_t slot) const override;
	virtual void bindConstantBuffer(ConstBufferSelection selection) const override;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void setFrontFace(FrontFace front) const override;
	virtual void setCullFunc(Culling cull) override;
	virtual void enable(Enablable property) const override;
	virtual void disable(Enablable property) const override;
	virtual void setDepthFunc(DepthTest testParam) const override;
	virtual void setBlendFunc(Blending blend) const override;
	virtual void setClearColor(float r, float g, float b, float a) const override;
	virtual void drawArrays(uint32_t index, uint32_t count, PrimitiveMode mode) const override;
	virtual void drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode mode) const override;
	virtual void resetFrame() const override;
	virtual void displayFrame() const override;
	virtual bool shouldClose() const override;
};
