#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "HuffmanNode.h"

class HuffmanTree
{
private:
    HuffmanNode *root;

    void deleteTree(HuffmanNode *node);

    void generateCodes(
        HuffmanNode *node,
        const std::string &code,
        std::unordered_map<unsigned char, std::string> &codes) const;

public:
    HuffmanTree();
    ~HuffmanTree();

    void build(const std::array<uint64_t, 256> &frequencies);

    std::unordered_map<unsigned char, std::string> getCodes() const;

    HuffmanNode *getRoot() const;

    bool empty() const;
};

#endif