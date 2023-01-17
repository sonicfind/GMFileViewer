#include "AttDefModel_Setup.h"

AttDefModel_Setup::AttDefModel_Setup(FilePointer& file) : Model_Setup(file)
{
	if (m_headerVersion < 0x1200)
		return;

	file.read(m_attackSize_Z);
	file.read(m_attackValues);
}
