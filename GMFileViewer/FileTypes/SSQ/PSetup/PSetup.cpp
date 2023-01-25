#include "PSetup.h"
#include "FileReader.h"

void PSetup::read(FileReader& file)
{
	if (!file.checkTag("PSTP"))
		throw "Unused Particle Setup read error";

	const uint32_t headerVersion = file.read<uint32_t>();
	file += 28;

	const uint32_t numMystery = file.read<uint32_t>();
	if (numMystery > 0)
		throw "Unsupported Particle Setup. Please submit the file to the GitarooPals discord for research.";
}
