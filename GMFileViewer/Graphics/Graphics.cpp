#include "OpenGL/Graphics_OGL.h"
#include "DirectX/Graphics_DX.h"
#include <stdexcept>

std::unique_ptr<Graphics> Graphics::s_gfx;
Graphics::GraphicsSettings Graphics::s_settings;

void Graphics::initGraphics(Backend backend)
{
	switch (backend)
	{
	case Graphics::Backend::DirectX:
		s_gfx = std::make_unique<Graphics_DX>();
		break;
	case Graphics::Backend::OpenGL:
		s_gfx = std::make_unique<Graphics_OGL>();
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
