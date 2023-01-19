#include "Graphics.h"
#include <stdexcept>

std::unique_ptr<Graphics> Graphics::s_gfx;

void Graphics::initGraphics(Backend backend)
{
	switch (backend)
	{
	case Graphics::Backend::DirectX:
		break;
	case Graphics::Backend::OpenGL:
		break;
	case Graphics::Backend::Vulkan:
		break;
	default:
		break;
	}
}

void Graphics::closeGraphics()
{
	s_gfx.reset();
}

Graphics* Graphics::getGraphics()
{
	if (!s_gfx)
		throw std::runtime_error("Graphics backend was not initialized");

	return s_gfx.get();
}
