#pragma once
#include "../PString.h"
#include "GMArray.h"
#include "Graphics.h"

template <PrimitiveMode MODE>
class Triangle_Data
{
	static_assert(MODE >= PrimitiveMode::TRIANGLE_LIST && MODE <= PrimitiveMode::PRIMITIVE);
protected:
	uint32_t m_numPrimitives = 0;
	GMArray<uint32_t> m_data;

public:
	void save(FileWriter& file) const
	{
		if      constexpr (MODE == PrimitiveMode::PRIMITIVE)      PString::WriteString("primCount", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_FAN)   PString::WriteString("triFanCount", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_STRIP) PString::WriteString("triStripCount", file);
		else                                                      PString::WriteString("triListCount", file);

		file.write(m_numPrimitives);

		if      constexpr (MODE == PrimitiveMode::PRIMITIVE)      PString::WriteString("primData", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_FAN)   PString::WriteString("triFanData", file);
		else if constexpr (MODE == PrimitiveMode::TRIANGLE_STRIP) PString::WriteString("triStripData", file);
		else                                                      PString::WriteString("triListData", file);

		m_data.write_full(file);
	}

	virtual void draw(Graphics& gfx) const = 0;

protected:
	Triangle_Data(FileReader& file)
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
};

template <PrimitiveMode MODE>
class Triangle_Group : public Triangle_Data<MODE>
{
public:
	Triangle_Group(FileReader& file)
		: Triangle_Data<MODE>(file) {}

	void draw(Graphics& gfx) const
	{
		if (this->m_data.isEmpty())
			return;

		gfx.setTopology(MODE);
		auto* curr = this->m_data.begin();
		
		uint32_t vertIndex = *curr++;
		for (uint32_t i = 0; i < this->m_numPrimitives; ++i)
		{
			const uint32_t count = *curr++;
			gfx.drawArrays(vertIndex, count);
			vertIndex += count;
		}
	}
};

template <PrimitiveMode MODE>
class Triangle_Separate : public Triangle_Data<MODE>
{
	size_t m_indexBuffer = 0;
	GMArray<std::pair<uint32_t, size_t>> countsAndOffsets;

public:
	Triangle_Separate(FileReader& file)
		: Triangle_Data<MODE>(file) {}

	void CreateIndexBuffer(Graphics& gfx)
	{
		const uint32_t numIndices = this->m_data.getSize() - this->m_numPrimitives;
		auto indices = std::make_unique<uint32_t[]>(numIndices);
		countsAndOffsets.reserve(this->m_numPrimitives);

		uint32_t* curr = this->m_data.begin();
		auto buff = (char*)indices.get();

		size_t offset = 0;
		for (uint32_t i = 0; i < this->m_numPrimitives; ++i)
		{
			const uint32_t numVerts = *curr++;
			memcpy(buff + offset, curr, numVerts);
			countsAndOffsets[i] = { numVerts, offset };

			curr += numVerts;
			offset += numVerts * sizeof(uint32_t);
		}

		m_indexBuffer = gfx.createIndexBuffer(indices.get(), numIndices);
	}

	void draw(Graphics& gfx) const
	{
		if (this->m_data.isEmpty())
			return;

		gfx.setTopology(MODE);
		gfx.bindIndexBuffer(m_indexBuffer);

		for (const auto& [count, offset] : countsAndOffsets)
			gfx.drawElements(count, offset);
	}
};
