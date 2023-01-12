#include "PSetup.h"
#include "FileOperations.h"

void PSetup::read(const char*& input)
{
	if (!FileOps::checkTag("PSTP", input))
		throw "Unused Particle Setup read error";

	const uint32_t headerVersion = FileOps::Read<uint32_t>(input);
	input += 28;

	const uint32_t numMystery = FileOps::Read<uint32_t>(input);
	if (numMystery > 0)
		throw "Unsupported Particle Setup. Please submit the file to the GitarooPals discord for research.";
}
