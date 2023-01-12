#pragma once
#include <stdint.h>

class IMXEntry
{
	alignas(16) char m_name[16] = { 0 };
	uint32_t m_isRGB; 
	uint32_t m_unused_2;
};
