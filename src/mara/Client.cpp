#include "Client.hpp"
using namespace std;

Client::Client(Server* server, int N){
    this->server = server;

    // Initialize a Dictionary Dx and the local stash
    Dx.clear();
    stash.clear();

    // Create and send BinaryTree to the Server
    this->bt = BinaryTree(N, kSO);
    this->server->setBinaryTree(&bt);
    this->soram = bt.getSORAM();

    // get first leafID and total number of leaves
    this->firstLeafID = soram->getFirstLeaf();
    this->totalLeaves = soram->getNumberOfLeaves();
}

Client::~Client(){
    delete soram;
}

void Client::update(map<string, dataBlocks> Data){
    for(auto i = Data.begin();i != Data.end();i++){
        int len = 32;
        unsigned char token[sizeof(char) * len];
        string input = i->second.keyword + to_string(Dx[i->second.keyword].first);
        char val[input.length()];
        strcpy(val, input.c_str());
        AES::ComputeToken(kh, sizeof(kh), val, token);

        Dx[i->second.keyword].second += 1;

        input = i->second.keyword + to_string(Dx[i->second.keyword].second);
        char val1[input.length()];
        strcpy(val1, input.c_str());
        unsigned long pth = AES::ComputePath(token, val1);

        i->second.pth = pth % totalLeaves + firstLeafID;

        Data[i->first] = {i->second.ind, i->second.keyword, i->second.op, i->second.pth};
    }
    insert(Data);
}

void Client::insert(map<string, dataBlocks> S){
    decrypted_nodes dec_nodes;
    encrypted_nodes enc_nodes = server->getNodesOnRandomPaths(S.size());
    // Decrypt nodes
    for(auto itr = enc_nodes.begin(); itr != enc_nodes.end(); itr++){
        dec_nodes.insert(make_pair(itr->first, soram->ReadBucket(itr->second, kSO)));
    }
    vector<int> J;

    for(auto it = dec_nodes.begin(); it != dec_nodes.end(); it++){
        J.push_back(it->first);
        for(int z = 0; z < Z; z++){
            if(it->second[z].data.ind != -1){ // It isn't a dummy block
                auto tmp = it->second[z].data;
                S[to_string(tmp.op) + to_string(tmp.ind) + tmp.keyword] = tmp;
            }
        }
    }
    enc_nodes.clear();
    dec_nodes.clear();

    // Inserts in S the data blocks from the stash (if any)
    for(auto i = stash.begin();i != stash.end();i++){
        S[i->first] = i->second;
    }
    stash.clear();

    S = Auxiliary::DelRedund(S);

    // Eviction algorithm
    dec_nodes = Auxiliary::Evict(S, J, &stash);

    // Encrypt nodes
    for(auto it = dec_nodes.begin(); it != dec_nodes.end(); it++){
        enc_nodes.insert(make_pair(it->first, soram->EncryptNode(it->second, kSO)));
    }
    server->insert(enc_nodes);
}

void Client::search(string keyword){
    remove(keyword);
    Dx[keyword].first += 1;
    Dx[keyword].second = 0;
}

void Client::remove(string keyword){
    map<string, dataBlocks> S;
    vector<int> J;
    int cUpd = Dx[keyword].second;
    int len = 32;
    unsigned char token[sizeof(char) * len];
    string input = keyword + to_string(Dx[keyword].first);
    char val[input.length()];
    strcpy(val, input.c_str());
    AES::ComputeToken(kh, sizeof(kh), val, token);

    // Decrypt nodes
    decrypted_nodes dec_nodes;
    encrypted_nodes enc_nodes = server->remove(keyword, token, cUpd);
    for(auto itr = enc_nodes.begin(); itr != enc_nodes.end(); itr++){
        dec_nodes.insert(make_pair(itr->first, soram->ReadBucket(itr->second, kSO)));
    }

    // insert dataBlocks in S
    for(auto it = dec_nodes.begin(); it != dec_nodes.end(); it++){
        J.push_back(it->first);
        for(int z = 0; z < Z; z++){
            if(it->second[z].data.ind != -1){ // It isn't a dummy block
                auto tmp = it->second[z].data;
                S[to_string(tmp.op) + to_string(tmp.ind) + tmp.keyword] = tmp;///////
            }
        }
    }
    enc_nodes.clear();
    dec_nodes.clear();

    // Inserts in S the data blocks from the stash (if any)
    for(auto i = stash.begin();i != stash.end();i++){
        S[i->first] = i->second;
    }
    stash.clear();

    S = Auxiliary::DelRedund(S);
    // remove from S the dataBlocks that contain keyword ω
    vector<string> found;
    for(auto block = S.begin(); block != S.end();block++){
        if(keyword.compare(block->second.keyword) == 0){
            //cout << "id = " << block->second.ind << " keyword = " << block->second.keyword << " op = " << block->second.op << " pth = " << block->second.pth << endl;
            found.push_back(block->first);
        }
    }
    for(auto block = found.begin(); block != found.end(); block++){
        S.erase(*block);
    }

    // Eviction algorithm
    dec_nodes = Auxiliary::Evict(S, J, &stash);

    // Encrypt nodes
    for(auto it = dec_nodes.begin(); it != dec_nodes.end(); it++){
        enc_nodes.insert(make_pair(it->first, soram->EncryptNode(it->second, kSO)));
    }
    server->insert(enc_nodes);
}
