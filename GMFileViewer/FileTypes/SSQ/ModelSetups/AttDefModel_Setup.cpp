#include "AttDefModel_Setup.h"

AttDefModel_Setup::AttDefModel_Setup(FilePointer& file) : Model_Setup(file)
{
	if (m_headerVersion < 0x1200)
		return;

	file.read(m_attackSize_Z);
	file.read(m_attackValues);
}

void AttDefModel_Setup::save(FileWriter& file) const
{
	Model_Setup::save(file);
	if (m_headerVersion < 0x1200)
		return;

	file.write(m_attackSize_Z);
	file.write(m_attackValues);
}
