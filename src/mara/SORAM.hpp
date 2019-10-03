#ifndef SORAM_H
#define SORAM_H

#include "AES.hpp"
#include <random>
#include <unordered_map>
#include <string>
#include <map>
#include <bits/stdc++.h>

using namespace std;

enum OP{
    DEL,INS
};

struct dataBlocks{
    int ind;
    string keyword;
    OP op;
    int pth;
};

struct Block {
    dataBlocks data;
};

using Bucket = std::array<Block, Z>;

class Node{
public:
    Node(){}
    ~Node(){}
    block value;
};

class SORAM {
    private:
        std::random_device rd;
        std::mt19937 mt;
        std::uniform_int_distribution<int> dis;

        Node* nodes;

        size_t depth;
        size_t clen_size;
        size_t blockSize;
        size_t plaintext_size;

        int nOfLeaves;
        int firstLeaf;
        int bucketCount;

        block SerialiseBucket(Bucket bucket);
        Bucket DeserialiseBucket(block buffer);
        void WriteBucket(int pos, Bucket bucket, bytes<Key> kSO);
    public:
        SORAM();
        ~SORAM();
        SORAM(int N, bytes<Key> key);

        block ReadBlock(int n);
        void setNode(int i, block value);
        Bucket ReadBucket(block ciphertext, bytes<Key> kSO);
        block EncryptNode(Bucket bucket, bytes<Key> kSO);

        int RandomPath();
        int getFirstLeaf();
        int getNumberOfLeaves();
};

#endif /* SORAM_H */
