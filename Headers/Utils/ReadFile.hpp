#ifndef __READFILE_HPP__
#define __READFILE_HPP__

#include <fstream>
#include <vector>

namespace Vox::Utils::Files
{

	inline std::vector<char> ReadFile(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (file.is_open() == false)
			throw std::runtime_error("Failed to open file : \'" + filename + "\'!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	inline std::vector<unsigned char> ReadFileUnsigned(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file)
		{
			throw std::runtime_error("Failed to open file : \'" + filename + "\'!");
			return {};
		}

		size_t fileSize = file.tellg();
		std::vector<unsigned char> buffer(fileSize);
		file.seekg(0);
		file.read((char *)buffer.data(), fileSize);
		return buffer;
	}
} // namespace Vox::Utils::Files

#endif // __READFILE_HPP__