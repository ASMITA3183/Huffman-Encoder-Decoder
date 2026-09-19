#ifndef HUFFMAN_FILE_FORMAT_H
#define HUFFMAN_FILE_FORMAT_H

#include <array>
#include <cstdint>
#include <fstream>

class HuffmanFileFormat
{
public:
    // Magic number used to identify a valid .huff file.
    static constexpr char MAGIC[4] = {
        'H', 'U', 'F', 'F'};

    // Write the header of a .huff file.
    static void writeHeader(
        std::ofstream &file,
        const std::array<uint64_t, 256> &frequencies,
        uint64_t originalSize);

    // Read and validate the header of a .huff file.
    static bool readHeader(
        std::ifstream &file,
        std::array<uint64_t, 256> &frequencies,
        uint64_t &originalSize);
};

#endif