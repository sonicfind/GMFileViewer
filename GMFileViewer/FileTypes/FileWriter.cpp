#include "FileWriter.h"

FileWriter::FileWriter(const std::filesystem::path& path) : m_file(path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc) {}

bool FileWriter::isOpen() const { return m_file.is_open(); }

std::streampos FileWriter::tell() { return m_file.tellp(); }

void FileWriter::seek(std::streampos pos) { m_file.seekp(pos); }
