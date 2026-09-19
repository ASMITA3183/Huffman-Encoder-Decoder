#include "FileReader.h"

#include "../format/HuffmanFileFormat.h"

#include <fstream>
#include <stdexcept>

FileReader::FileReader(const std::string &filename)
    : filename(filename)
{
}

std::vector<unsigned char> FileReader::readAll() const
{

    std::ifstream file(
        filename,
        std::ios::binary | std::ios::ate);

    if (!file)
    {
        throw std::runtime_error(
            "Unable to open input file: " + filename);
    }

    std::streamsize size = file.tellg();

    if (size < 0)
    {
        throw std::runtime_error(
            "Unable to determine file size.");
    }

    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> data(
        static_cast<size_t>(size));

    if (size > 0)
    {

        file.read(
            reinterpret_cast<char *>(data.data()),
            size);
    }

    return data;
}

bool FileReader::readCompressedFile(
    std::array<uint64_t, 256> &frequencies,
    uint64_t &originalSize,
    std::vector<unsigned char> &encodedData) const
{

    std::ifstream file(
        filename,
        std::ios::binary);

    if (!file)
    {
        return false;
    }

    // Read and validate the .huff header
    if (!HuffmanFileFormat::readHeader(
            file,
            frequencies,
            originalSize))
    {

        return false;
    }

    // Read the remaining bytes as compressed data
    encodedData.clear();

    char byte;

    while (file.get(byte))
    {

        encodedData.push_back(
            static_cast<unsigned char>(byte));
    }

    return true;
}