#include "BinaryTree.h"

using namespace std;

BinaryTree::BinaryTree(int N, bytes<Key> key){
    soram = new SORAM(N, key);
}

BinaryTree::BinaryTree(){
}

BinaryTree::~BinaryTree(){
}

// extract the unique nodes from nb randomly selected tree paths
encrypted_nodes BinaryTree::extractRandNodes(int nb){
    encrypted_nodes map;
    while(nb > 0){
        int leaf = soram->RandomPath();
        int nodeID = leaf;
        while(nodeID >= 0){
            auto notInserted = map.insert(make_pair(nodeID, soram->ReadBlock(nodeID)));
            if(notInserted.second == true){
                if(leaf == nodeID) nb--;
                if(nodeID % 2 == 0){
                    nodeID = nodeID / 2 - 1;
                }else{
                    nodeID = nodeID / 2;
                }
            }else{ // node already inserted
                break;
            }
        }
    }
    return map;
}

// extract all nodes that belong in the J
encrypted_nodes BinaryTree::extractNodes(vector<int> J){
    encrypted_nodes map;

    for(auto it = J.begin(); it != J.end(); it++){
        int n = *it;
        while(n >= 0){
            auto notInserted = map.insert(make_pair(n, soram->ReadBlock(n)));
            if(notInserted.second == true){
                if(n % 2 == 0){ // right child
                    n = n / 2 - 1;
                }else{ // left child
                    n = n / 2;
                }
            }else{ // node already inserted
                break;
            }
        }
    }
    return map;
}

// insert the nodes in the tree
// replacing their previous content
void BinaryTree::insertNodes(encrypted_nodes nodes){
    for(auto it = nodes.begin(); it != nodes.end(); it++){
        soram->setNode(it->first, it->second);
    }
}

SORAM* BinaryTree::getSORAM(){
    return this->soram;
}

int BinaryTree::getNumberOfLeaves(){
    return soram->getNumberOfLeaves();
}

int BinaryTree::getFirstLeaf(){
    return soram->getFirstLeaf();
}
