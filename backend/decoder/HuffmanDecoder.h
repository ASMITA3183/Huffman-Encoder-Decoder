#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include <string>

class HuffmanDecoder
{
public:
    void decompress(
        const std::string &inputFile,
        const std::string &outputFile);
};

#endif