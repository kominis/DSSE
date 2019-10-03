#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "SORAM.hpp"

using namespace std;

typedef map<int, Bucket> decrypted_nodes;
typedef map<int, block> encrypted_nodes;

class BinaryTree {
    private:
        SORAM* soram;
    public:
        BinaryTree(int N, bytes<Key> key);
        BinaryTree();
        ~BinaryTree();

        void insertNodes(encrypted_nodes nodes);

        encrypted_nodes extractRandNodes(int nb);
        encrypted_nodes extractNodes(vector<int> J);

        SORAM* getSORAM();
        int getNumberOfLeaves();
        int getFirstLeaf();
};

#endif /* BINARYTREE_H */
