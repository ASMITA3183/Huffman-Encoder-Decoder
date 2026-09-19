#include "HuffmanNode.h"

HuffmanNode::HuffmanNode(
    unsigned char data,
    uint64_t frequency)
    : data(data),
      frequency(frequency),
      left(nullptr),
      right(nullptr),
      minData(data)
{
}

HuffmanNode::HuffmanNode(
    uint64_t frequency,
    HuffmanNode *left,
    HuffmanNode *right)
    : data(0),
      frequency(frequency),
      left(left),
      right(right),
      minData(0)
{

    if (left != nullptr && right != nullptr)
    {

        minData = (left->minData < right->minData)
                      ? left->minData
                      : right->minData;
    }
}

bool HuffmanNode::isLeaf() const
{
    return left == nullptr && right == nullptr;
}