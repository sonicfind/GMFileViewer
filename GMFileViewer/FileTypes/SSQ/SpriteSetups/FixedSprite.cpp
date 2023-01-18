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
void FixedSprite::read(FilePointer& file)
{

	if (!file.checkTag("GMSP"))
		throw "Sprite Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	file.read(m_64bytes);

	unsigned long numWorldValues, numColors, numFrames;
	file.read(numWorldValues);
	file.read(numColors);
	file.read(numFrames);

	if (numWorldValues >= 2)
		m_worldValues.reserve_and_fill(file, numWorldValues);

	if (numColors >= 2)
		m_colors.reserve_and_fill(file, numColors);

	if (numFrames > 2)
		m_spriteFrames.reserve_and_fill(file, numFrames);
}
