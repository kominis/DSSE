#ifndef AUXILIARY_H
#define AUXILIARY_H
#include "BinaryTree.h"

using namespace std;

class Auxiliary {
    private:
        static bool Pth(int pth, int i);

    public:
        Auxiliary();
        virtual ~Auxiliary();

        static std::map<int, std::chrono::time_point<std::chrono::high_resolution_clock> > m_begs;

        static void startTimer(int id);
        static double stopTimer(int id);

        static map<string, dataBlocks> DelRedund(map<string, dataBlocks> S);
        static decrypted_nodes Evict(map<string, dataBlocks> S, vector<int> J, map<string, dataBlocks>* stash);
};

#endif /* AUXILIARY_H */
