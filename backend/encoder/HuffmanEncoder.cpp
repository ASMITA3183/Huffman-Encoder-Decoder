#include "HuffmanEncoder.h"

#include "../file/FileReader.h"
#include "../file/FileWriter.h"
#include "../tree/HuffmanTree.h"

#include <array>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

void HuffmanEncoder::compress(
    const std::string &inputFile,
    const std::string &outputFile)
{
    FileReader reader(inputFile);

    std::vector<unsigned char> data = reader.readAll();

    std::array<uint64_t, 256> frequencies{};
    frequencies.fill(0);

    for (unsigned char byte : data)
    {
        frequencies[byte]++;
    }

    HuffmanTree tree;
    tree.build(frequencies);

    std::unordered_map<unsigned char, std::string> codes =
        tree.getCodes();

    std::vector<unsigned char> encodedData;

    unsigned char currentByte = 0;
    int bitCount = 0;

    for (unsigned char byte : data)
    {
        const std::string &code = codes.at(byte);

        for (char bit : code)
        {
            currentByte <<= 1;

            if (bit == '1')
            {
                currentByte |= 1;
            }

            bitCount++;

            if (bitCount == 8)
            {
                encodedData.push_back(currentByte);
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    // Handle remaining bits
    if (bitCount > 0)
    {
        currentByte <<= (8 - bitCount);
        encodedData.push_back(currentByte);
    }

    FileWriter writer(outputFile);

    writer.writeCompressedFile(
        frequencies,
        static_cast<uint64_t>(data.size()),
        encodedData);
}