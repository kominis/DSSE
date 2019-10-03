#include "Server.h"

using namespace std;

Server::Server(){
}

Server::~Server() {
}

// Returns the nodes that belong
// in nb randomly selected paths
encrypted_nodes Server::getNodesOnRandomPaths(int nb){
    return bt->extractRandNodes(nb);
}

void Server::insert(encrypted_nodes nodes){
    bt->insertNodes(nodes);
}

encrypted_nodes Server::remove(string keyword, unsigned char token[], int cUpd){
    vector<int> J;
    int numberOfLeaves = bt->getNumberOfLeaves();
    int firstLeafID = bt->getFirstLeaf();
    for(int i=1; i <= cUpd; i++){
        string input = keyword + to_string(i);
        char val1[input.length()];
        strcpy(val1, input.c_str());
        int pth = AES::ComputePath(token, val1);
        pth = pth % numberOfLeaves + firstLeafID;
        J.push_back(pth);
    }
    return bt->extractNodes(J);
}

void Server::setBinaryTree(BinaryTree* bt){
    this->bt = bt;
}
