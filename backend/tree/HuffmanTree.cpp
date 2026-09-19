#include "HuffmanTree.h"

#include <queue>
#include <vector>

namespace
{

    struct CompareNodes
    {

        bool operator()(
            HuffmanNode *a,
            HuffmanNode *b) const
        {

            // Lower frequency gets higher priority.
            if (a->frequency != b->frequency)
            {
                return a->frequency > b->frequency;
            }

            // If frequencies are equal, use minData
            // as a deterministic tie-breaker.
            return a->minData > b->minData;
        }
    };

}

HuffmanTree::HuffmanTree()
    : root(nullptr)
{
}

HuffmanTree::~HuffmanTree()
{
    deleteTree(root);
}

void HuffmanTree::deleteTree(HuffmanNode *node)
{

    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

void HuffmanTree::build(
    const std::array<uint64_t, 256> &frequencies)
{

    deleteTree(root);
    root = nullptr;

    std::priority_queue<
        HuffmanNode *,
        std::vector<HuffmanNode *>,
        CompareNodes>
        minHeap;

    for (int i = 0; i < 256; ++i)
    {

        if (frequencies[i] > 0)
        {

            minHeap.push(
                new HuffmanNode(
                    static_cast<unsigned char>(i),
                    frequencies[i]));
        }
    }

    if (minHeap.empty())
    {
        return;
    }

    if (minHeap.size() == 1)
    {

        root = minHeap.top();

        minHeap.pop();

        return;
    }

    while (minHeap.size() > 1)
    {

        HuffmanNode *left =
            minHeap.top();

        minHeap.pop();

        HuffmanNode *right =
            minHeap.top();

        minHeap.pop();

        HuffmanNode *parent =
            new HuffmanNode(
                left->frequency +
                    right->frequency,
                left,
                right);

        minHeap.push(parent);
    }

    root = minHeap.top();

    minHeap.pop();
}

void HuffmanTree::generateCodes(
    HuffmanNode *node,
    const std::string &code,
    std::unordered_map<
        unsigned char,
        std::string> &codes) const
{

    if (node == nullptr)
    {
        return;
    }

    if (node->isLeaf())
    {

        codes[node->data] =
            code.empty() ? "0" : code;

        return;
    }

    generateCodes(
        node->left,
        code + "0",
        codes);

    generateCodes(
        node->right,
        code + "1",
        codes);
}

std::unordered_map<
    unsigned char,
    std::string>
HuffmanTree::getCodes() const
{

    std::unordered_map<
        unsigned char,
        std::string>
        codes;

    if (root != nullptr)
    {

        generateCodes(
            root,
            "",
            codes);
    }

    return codes;
}

HuffmanNode *HuffmanTree::getRoot() const
{
    return root;
}

bool HuffmanTree::empty() const
{
    return root == nullptr;
}