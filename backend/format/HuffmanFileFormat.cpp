#include "HuffmanFileFormat.h"

#include <cstring>

constexpr char HuffmanFileFormat::MAGIC[4];

void HuffmanFileFormat::writeHeader(
    std::ofstream &file,
    const std::array<uint64_t, 256> &frequencies,
    uint64_t originalSize)
{
    // -----------------------------------
    // 1. Write magic number
    // -----------------------------------

    file.write(
        MAGIC,
        sizeof(MAGIC));

    // -----------------------------------
    // 2. Write original file size
    // -----------------------------------

    file.write(
        reinterpret_cast<const char *>(&originalSize),
        sizeof(originalSize));

    // -----------------------------------
    // 3. Write frequency table
    // -----------------------------------

    file.write(
        reinterpret_cast<const char *>(frequencies.data()),
        sizeof(uint64_t) * frequencies.size());
}

bool HuffmanFileFormat::readHeader(
    std::ifstream &file,
    std::array<uint64_t, 256> &frequencies,
    uint64_t &originalSize)
{
    // -----------------------------------
    // 1. Read magic number
    // -----------------------------------

    char magic[4];

    file.read(
        magic,
        sizeof(magic));

    if (!file)
    {
        return false;
    }

    // -----------------------------------
    // 2. Validate magic number
    // -----------------------------------

    if (std::memcmp(
            magic,
            MAGIC,
            sizeof(MAGIC)) != 0)
    {

        return false;
    }

    // -----------------------------------
    // 3. Read original file size
    // -----------------------------------

    file.read(
        reinterpret_cast<char *>(&originalSize),
        sizeof(originalSize));

    if (!file)
    {
        return false;
    }

    // -----------------------------------
    // 4. Read frequency table
    // -----------------------------------

    frequencies.fill(0);

    file.read(
        reinterpret_cast<char *>(frequencies.data()),
        sizeof(uint64_t) * frequencies.size());

    if (!file)
    {
        return false;
    }

    return true;
}