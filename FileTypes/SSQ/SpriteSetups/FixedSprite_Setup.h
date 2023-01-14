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
#include "FixedSprite.h"
#include "../SSQModelType.h"
class FixedSpriteSetup
{
	struct Struct80_7f
	{
		unsigned long IMXindex;
		DirectX::XMFLOAT2 initial_BottomLeft;
		DirectX::XMFLOAT2 boxSize;
		DirectX::XMFLOAT3 worldPosition;
		DirectX::XMFLOAT2 worldSize;
		DirectX::XMFLOAT2 worldSize_ZW_maybe;
		float float_l;
		unsigned long ulong_b;
		float float_m;
		unsigned long ulong_c;
		unsigned long depthTest;
		SSQModelType modelTypeMapping = SSQModelType::Normal;
		unsigned long ulong_f;
		unsigned long ulong_g;
	};

	GMArray<Struct80_7f> m_80bytes;
	GMArray<FixedSprite> m_fixedSprites;

public:
	void read(const char*& input);
};
