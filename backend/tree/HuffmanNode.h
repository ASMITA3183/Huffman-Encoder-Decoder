#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <cstdint>

class HuffmanNode
{
public:
    unsigned char data;
    uint64_t frequency;

    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(
        unsigned char data,
        uint64_t frequency);

    HuffmanNode(
        uint64_t frequency,
        HuffmanNode *left,
        HuffmanNode *right);

    bool isLeaf() const;

    // Smallest byte value contained in this subtree.
    // Used to make Huffman tree construction deterministic.
    unsigned char minData;
};

#endif