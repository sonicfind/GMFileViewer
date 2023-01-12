#include "AttDefModel_Setup.h"

AttDefModel_Setup::AttDefModel_Setup(const char*& input) : Model_Setup(input)
{
	if (m_headerVersion < 0x1200)
		return;

	FileOps::Read(m_attackSize_Z, input);
	FileOps::Read(m_attackValues, input);
}
