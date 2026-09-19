#ifndef FILE_READER_H
#define FILE_READER_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class FileReader
{
private:
    std::string filename;

public:
    explicit FileReader(const std::string &filename);

    std::vector<unsigned char> readAll() const;

    bool readCompressedFile(
        std::array<uint64_t, 256> &frequencies,
        uint64_t &originalSize,
        std::vector<unsigned char> &encodedData) const;
};

#endif