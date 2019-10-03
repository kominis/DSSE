#ifndef SERVER_H
#define SERVER_H
#include "BinaryTree.h"
#include "Auxiliary.h"

using namespace std;

class Server {
    private:
        BinaryTree* bt;

    public:
        Server();
        virtual ~Server();

        void setBinaryTree(BinaryTree* bt);
        void insert(encrypted_nodes nodes);
        encrypted_nodes remove(string keyword, unsigned char token[], int cUpd);
        encrypted_nodes getNodesOnRandomPaths(int nb);
};

#endif /* SERVER_H */
