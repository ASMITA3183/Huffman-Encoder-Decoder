#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class FileWriter
{
private:
    std::string filename;

public:
    explicit FileWriter(const std::string &filename);

    void writeCompressedFile(
        const std::array<uint64_t, 256> &frequencies,
        uint64_t originalSize,
        const std::vector<unsigned char> &encodedData) const;
};

#endif