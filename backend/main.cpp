#include "encoder/HuffmanEncoder.h"
#include "decoder/HuffmanDecoder.h"

#include <exception>
#include <iostream>
#include <string>

void printUsage()
{
    std::cout << "\nHuffman File Compressor\n\n";

    std::cout << "Usage:\n";

    std::cout
        << "  ./huffman compress <input> <output>\n";

    std::cout
        << "  ./huffman decompress <input> <output>\n\n";

    std::cout << "Examples:\n";

    std::cout
        << "  ./huffman compress test.txt compressed.huf\n";

    std::cout
        << "  ./huffman decompress compressed.huf restored.txt\n\n";
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printUsage();
        return 1;
    }

    std::string operation = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    try
    {

        if (operation == "compress")
        {

            HuffmanEncoder encoder;

            encoder.compress(
                inputFile,
                outputFile);

            std::cout
                << "Compression completed successfully.\n";

            std::cout
                << "Input : " << inputFile << '\n';

            std::cout
                << "Output: " << outputFile << '\n';
        }
        else if (operation == "decompress")
        {

            HuffmanDecoder decoder;

            decoder.decompress(
                inputFile,
                outputFile);

            std::cout
                << "Decompression completed successfully.\n";

            std::cout
                << "Input : " << inputFile << '\n';

            std::cout
                << "Output: " << outputFile << '\n';
        }
        else
        {

            std::cerr
                << "Unknown operation: "
                << operation
                << '\n';

            printUsage();

            return 1;
        }
    }
    catch (const std::exception &e)
    {

        std::cerr
            << "Error: "
            << e.what()
            << '\n';

        return 1;
    }

    return 0;
}