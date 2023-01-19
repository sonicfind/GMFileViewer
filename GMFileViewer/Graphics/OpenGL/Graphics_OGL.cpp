#include "Graphics_OGL.h"

void Graphics_OGL::activateShader(ShaderType type) const
{
}

size_t Graphics_OGL::createVertexBuffer(const void* data, uint32_t dataSize, bool isDynamic)
{
    return size_t();
}

void Graphics_OGL::bindVertexBuffer(size_t index) const
{
}

void Graphics_OGL::updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
}

void Graphics_OGL::createTexture(const char(&name)[16], const void* imagePtr, uint32_t width, uint32_t height)
{
}

void Graphics_OGL::bindTexture(const char(&name)[16]) const
{
}

void Graphics_OGL::updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const
{
}

void Graphics_OGL::unbindTexture() const
{
}

void Graphics_OGL::bindConstantBuffer(ConstBufferSelection selection) const
{
}

void Graphics_OGL::updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const
{
}

void Graphics_OGL::updateCameraBuffers(const float* viewMatrix, const float* projectionMatrix) const
{
}

void Graphics_OGL::drawArrays(uint32_t index, uint32_t count, PrimitiveMode type) const
{
}

void Graphics_OGL::drawElements(uint32_t count, const uint32_t* indices, PrimitiveMode type) const
{
}

void Graphics_OGL::setCullFunc(Culling cull) const
{
}

void Graphics_OGL::resetFrame() const
{
}

void Graphics_OGL::displayFrame() const
{
}
