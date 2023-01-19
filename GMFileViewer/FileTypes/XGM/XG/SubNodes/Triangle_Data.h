#pragma once
#include "../PString.h"
#include "GMArray.h"
#include "Graphics.h"

template <PrimitiveMode MODE>
class Triangle_Data
{
	static_assert(MODE >= PrimitiveMode::PRIMITIVE && MODE <= PrimitiveMode::TRIANGLE_LIST);
protected:
	uint32_t m_numPrimitives = 0;
	GMArray<uint32_t> m_data;

public:
	void load(FilePointer& file)
	{
		if      constexpr (MODE == PrimitiveMode::PRIMITIVE)      PString::ThrowOnStringMismatch("primCount", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_FAN)   PString::ThrowOnStringMismatch("triFanCount", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_STRIP) PString::ThrowOnStringMismatch("triStripCount", file);
		else                                                      PString::ThrowOnStringMismatch("triListCount", file);

		file.read(m_numPrimitives);

		if      constexpr (MODE == PrimitiveMode::PRIMITIVE)      PString::ThrowOnStringMismatch("primData", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_FAN)   PString::ThrowOnStringMismatch("triFanData", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_STRIP) PString::ThrowOnStringMismatch("triStripData", file);
		else                                                      PString::ThrowOnStringMismatch("triListData", file);

		m_data.reserve_and_fill(file);
	}

	template <uint32_t PRIMTYPE>
	void draw() const
	{
		if (m_data.isEmpty())
			return;

		const Graphics* gfx = Graphics::getGraphics();
		auto* curr = m_data.begin();
		auto* const end = m_data.end();
		if constexpr (PRIMTYPE == 4)
		{
			while (curr < end)
			{
				const uint32_t numVerts = *curr++;
				gfx->drawElements(numVerts, curr, MODE);
				curr += numVerts;
			}
		}
		else if constexpr (PRIMTYPE == 5)
		{
			uint32_t vertIndex = *curr++;
			while (curr < end)
			{
				const uint32_t count = *curr++;
				gfx->drawArrays(vertIndex, count, MODE);
				vertIndex += count;
			}
		}
	}
};
