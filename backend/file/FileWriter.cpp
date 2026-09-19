#include "FileWriter.h"

#include "../format/HuffmanFileFormat.h"

#include <fstream>
#include <stdexcept>

FileWriter::FileWriter(const std::string &filename)
    : filename(filename)
{
}

void FileWriter::writeCompressedFile(
    const std::array<uint64_t, 256> &frequencies,
    uint64_t originalSize,
    const std::vector<unsigned char> &encodedData) const
{

    std::ofstream file(
        filename,
        std::ios::binary);

    if (!file)
    {
        throw std::runtime_error(
            "Unable to create output file: " + filename);
    }

    // Write the .huff file header
    HuffmanFileFormat::writeHeader(
        file,
        frequencies,
        originalSize);

    // Write compressed binary data
    if (!encodedData.empty())
    {

        file.write(
            reinterpret_cast<const char *>(
                encodedData.data()),
            static_cast<std::streamsize>(
                encodedData.size()));
    }

    if (!file)
    {
        throw std::runtime_error(
            "Error while writing compressed file.");
    }
}