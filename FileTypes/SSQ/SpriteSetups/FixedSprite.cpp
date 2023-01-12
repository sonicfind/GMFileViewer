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
void FixedSprite::read(const char*& input)
{

	if (!FileOps::checkTag("GMSP", input))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);
	input += 28;

	FileOps::Read(m_64bytes, input);

	unsigned long numWorldValues, numColors, numFrames;
	FileOps::Read(numWorldValues, input);
	FileOps::Read(numColors, input);
	FileOps::Read(numFrames, input);

	if (numWorldValues >= 2)
		m_worldValues.reserve_and_fill(input, numWorldValues);

	if (numColors >= 2)
		m_colors.reserve_and_fill(input, numColors);

	if (numFrames > 2)
		m_spriteFrames.reserve_and_fill(input, numFrames);
}
