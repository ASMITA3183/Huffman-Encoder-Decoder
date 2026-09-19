#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <string>

class HuffmanEncoder
{
public:
    void compress(
        const std::string &inputFile,
        const std::string &outputFile);
};

#endif