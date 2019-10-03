#include "Auxiliary.h"

using namespace std;

std::map<int, std::chrono::time_point<std::chrono::high_resolution_clock>> Auxiliary::m_begs;
std::map<int, double> timehist;

Auxiliary::Auxiliary(){
}

Auxiliary::~Auxiliary(){
}

decrypted_nodes Auxiliary::Evict(map<string, dataBlocks> S, vector<int> J, map<string, dataBlocks>* stash){
    std::random_device rd;
    std::mt19937 mt(rd());
    decrypted_nodes nodes;

    for(auto itr = J.rbegin(); itr != J.rend(); itr++){
        map<string, dataBlocks> temp;
        for(auto block = S.begin(); block != S.end();block++){
            if(Pth(block->second.pth, *itr)){
                temp[block->first] = {block->second};
            }
        }
        Bucket bucket;
        for(int z = 0; z < Z; z++){
            // Insert in the bucket random blocks from S
            if(temp.size() != 0 && S.size() != 0){
                auto dis = std::uniform_int_distribution<int>{0, (int)temp.size() - 1};
                int rnd = dis(mt);
                auto it = temp.begin();
                std::advance(it, rnd);
                bucket[z] = {it->second};
                S.erase(it->first);
                temp.erase(it);
            }else{ // dummy block
                bucket[z].data.ind = -1;
                bucket[z].data.keyword = "dummydata";
                bucket[z].data.op = OP::INS;
                bucket[z].data.pth = -1;
            }
        }
        nodes.insert(make_pair(*itr, bucket));
    }
    for(auto block=S.begin();block != S.end(); block++) {
        (*stash)[block->first] = block->second;
    }
    return nodes;
}

// return true if node i exists in path pth
bool Auxiliary::Pth(int pth, int i){
    if(i == 0 || pth == i) return true;
    if(pth < i) return false;
    if(pth % 2 == 0) return Pth(pth / 2 - 1, i); // right child
    if(pth % 2 == 1) return Pth(pth / 2, i); // left child

    return false;
}

map<string, dataBlocks> Auxiliary::DelRedund(map<string, dataBlocks> S){
    vector<dataBlocks> tempS;
    map<string, int> dictionary;

    for(auto block = S.begin();block != S.end();block++) {
        dataBlocks b;
        b.ind = block->second.ind;
        b.keyword = block->second.keyword;
        b.pth = block->second.pth;
        tempS.push_back({b});
        dictionary[to_string(b.ind) + b.keyword] += 2 * static_cast<int>(block->second.op) - 1;
    }

    S.clear();
    for(int i = 0;i < (int)tempS.size();i++){
        if(dictionary[to_string(tempS[i].ind) + tempS[i].keyword] != 0){
            dataBlocks b;
            b.ind = tempS[i].ind;
            b.keyword = tempS[i].keyword;
            b.pth = tempS[i].pth;
            b.op = static_cast<OP>((dictionary[to_string(b.ind) + b.keyword] + 1) / 2);
            S[to_string(b.op) + to_string(b.ind) + b.keyword] = {b};
        }
    }

    return S;
}

void Auxiliary::startTimer(int id){
    std::chrono::time_point<std::chrono::high_resolution_clock> m_beg = std::chrono::high_resolution_clock::now();
    m_begs[id] = m_beg;
}

double Auxiliary::stopTimer(int id){
    double t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begs[id]).count();
    timehist.erase(id);
    timehist[id] = t;
    return t;
}
