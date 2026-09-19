#include "HuffmanDecoder.h"

#include "../file/FileReader.h"
#include "../tree/HuffmanTree.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <vector>

void HuffmanDecoder::decompress(
    const std::string &inputFile,
    const std::string &outputFile)
{
    FileReader reader(inputFile);

    std::array<uint64_t, 256> frequencies{};
    uint64_t originalSize = 0;

    std::vector<unsigned char> encodedData;

    bool success = reader.readCompressedFile(
        frequencies,
        originalSize,
        encodedData);

    if (!success)
    {
        throw std::runtime_error(
            "Invalid or corrupted Huffman file.");
    }

    std::ofstream output(
        outputFile,
        std::ios::binary);

    if (!output)
    {
        throw std::runtime_error(
            "Unable to create output file: " + outputFile);
    }

    // Empty original file
    if (originalSize == 0)
    {
        return;
    }

    HuffmanTree tree;
    tree.build(frequencies);

    if (tree.empty())
    {
        throw std::runtime_error(
            "Invalid Huffman tree.");
    }

    // Special case:
    // only one unique character
    if (tree.getRoot()->isLeaf())
    {
        for (uint64_t i = 0; i < originalSize; ++i)
        {
            output.put(
                static_cast<char>(tree.getRoot()->data));
        }

        return;
    }

    HuffmanNode *current = tree.getRoot();

    uint64_t decodedCount = 0;

    for (unsigned char byte : encodedData)
    {
        for (int bit = 7; bit >= 0; --bit)
        {
            int value = (byte >> bit) & 1;

            if (value == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }

            if (current == nullptr)
            {
                throw std::runtime_error(
                    "Corrupted compressed data.");
            }

            if (current->isLeaf())
            {
                output.put(
                    static_cast<char>(current->data));

                decodedCount++;

                if (decodedCount == originalSize)
                {
                    return;
                }

                current = tree.getRoot();
            }
        }
    }

    if (decodedCount != originalSize)
    {
        throw std::runtime_error(
            "Compressed file ended before decoding completed.");
    }
}