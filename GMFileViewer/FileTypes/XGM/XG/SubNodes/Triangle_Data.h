#pragma once
#include "../PString.h"
#include "GMArray.h"

enum class PrimitiveType
{
	PRIMITIVE,
	TRIANGLE_FAN,
	TRIANGLE_STRIP,
	TRIANGLE_LIST,
};

template <PrimitiveType MODE>
class Triangle_Data
{
	static_assert(MODE >= PrimitiveType::PRIMITIVE && MODE <= PrimitiveType::TRIANGLE_LIST);
protected:
	uint32_t m_numPrimitives = 0;
	GMArray<uint32_t> m_data;

public:
	void load(FilePointer& file)
	{
		if      constexpr (MODE == PrimitiveType::PRIMITIVE)      PString::ThrowOnStringMismatch("primCount", file);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_FAN)   PString::ThrowOnStringMismatch("triFanCount", file);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_STRIP) PString::ThrowOnStringMismatch("triStripCount", file);
		else                                                      PString::ThrowOnStringMismatch("triListCount", file);

		file.read(m_numPrimitives);

		if      constexpr (MODE == PrimitiveType::PRIMITIVE)      PString::ThrowOnStringMismatch("primData", file);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_FAN)   PString::ThrowOnStringMismatch("triFanData", file);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_STRIP) PString::ThrowOnStringMismatch("triStripData", file);
		else                                                      PString::ThrowOnStringMismatch("triListData", file);

		m_data.reserve_and_fill(file);
	}

	template <uint32_t PRIMTYPE>
	void draw() const
	{
		if (m_data.isEmpty())
			return;

		if constexpr (PRIMTYPE == 4)
		{
			for (size_t i = 0; i < m_data.getSize();)
			{
				const uint32_t numVerts = m_data[i++];

				/* Draw Elements Here */

				i += numVerts;
			}
		}
		else if constexpr (PRIMTYPE == 5)
		{
			uint32_t vertIndex = m_data.front();
			for (size_t i = 1; i < m_data.getSize(); ++i)
			{
				const uint32_t count = m_data[i];
				/* Draw Array Here */
				vertIndex += count;
			}
		}
	}
};
