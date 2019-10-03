#ifndef CLIENT_H
#define CLIENT_H
#include "Server.h"
#include "SORAM.hpp"
#include "Auxiliary.h"
#include "BinaryTree.h"

using namespace std;

class Client {
    private:
        BinaryTree bt;
        SORAM* soram;
        Server* server;

        int firstLeafID, totalLeaves;

        bytes<Key> kSO {{0}}; // SORAM Key
        unsigned char kh[Blake2Key] {1}; // PRF Key(Blake2)

        unordered_map<string, pair<int, int>> Dx; //Dictionary
        map<string, dataBlocks> stash;

        void remove(string keyword);
        void insert(map<string, dataBlocks> B);
    public:
        Client(Server* server, int N);
        virtual ~Client();

        void update(map<string, dataBlocks> Data);
        void search(string keyword);

};

#endif /* CLIENT_H */
