#pragma once
/*  Gitaroo Man File Reader
 *  Copyright (C) 2020 Gitaroo Pals
 *
 *  Gitaroo Man File Reader is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  Gitaroo Man File Reader is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with Gitaroo Man File Reader.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../PString.h"
#include "FileOperations.h"
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
	void load(const char*& input)
	{
		if      constexpr (MODE == PrimitiveType::PRIMITIVE)      PString::CheckForString("primCount", input);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_FAN)   PString::CheckForString("triFanCount", input);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_STRIP) PString::CheckForString("triStripCount", input);
		else                                                      PString::CheckForString("triListCount", input);

		FileOps::Read(m_numPrimitives, input);

		if      constexpr (MODE == PrimitiveType::PRIMITIVE)      PString::CheckForString("primData", input);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_FAN)   PString::CheckForString("triFanData", input);
		else if constexpr (MODE == PrimitiveType::TRIANGLE_STRIP) PString::CheckForString("triStripData", input);
		else                                                      PString::CheckForString("triListData", input);

		m_data.reserve_and_fill(input);
	}
};
