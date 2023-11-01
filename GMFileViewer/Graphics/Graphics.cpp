#include "Graphics.h"

Graphics::Graphics(std::string window, uint32_t width, uint32_t height, uint32_t maxFPS)
	: m_settings({window, width, height, maxFPS}) {}