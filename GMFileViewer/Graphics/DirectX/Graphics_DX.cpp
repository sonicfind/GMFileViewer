#include "Graphics_DX.h"

void Graphics_DX::activateShader(ShaderType type) const
{
}

size_t Graphics_DX::createVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic)
{
    return size_t();
}

void Graphics_DX::bindVertexBuffer(size_t index) const
{
}

void Graphics_DX::updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
}

void Graphics_DX::createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height)
{
}

void Graphics_DX::bindTexture(std::string_view name) const
{
}

void Graphics_DX::updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const
{
}

void Graphics_DX::unbindTexture() const
{
}

void Graphics_DX::bindConstantBuffer(ConstBufferSelection selection) const
{
}

void Graphics_DX::updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
}

void Graphics_DX::updateCameraBuffers(const float* viewMatrix, const float* projectionMatrix) const
{
}

void Graphics_DX::setCullFunc(Culling cull)
{
}

void Graphics_DX::setDepthTest(bool enable) const
{
}

void Graphics_DX::setDepthFunc(DepthTest testParam) const
{
}

void Graphics_DX::drawArrays(uint32_t index, uint32_t count, PrimitiveMode type) const
{
}

void Graphics_DX::drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode type) const
{
}

void Graphics_DX::resetFrame() const
{
}

void Graphics_DX::displayFrame() const
{
}
