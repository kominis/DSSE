#include "SORAM.hpp"
#include <bits/stdc++.h>
#include "Client.hpp"

using namespace std;

SORAM::SORAM(int N, bytes<Key> kSO)
: rd(), mt(rd()){
    AES::Setup();
    depth = ceil(log2(N));
    bucketCount = pow(2, depth + 1) - 1;
    blockSize = sizeof(string);
    nOfLeaves = bucketCount / 2 + 1;
    firstLeaf = bucketCount - nOfLeaves;
    nodes = new Node[bucketCount];
    dis = std::uniform_int_distribution<int>{firstLeaf, bucketCount - 1};
    clen_size = AES::GetCiphertextLength(blockSize * Z);
    plaintext_size = blockSize * Z;
    for(int i = 0; i < bucketCount; i++){
        Bucket bucket;
        for(int z = 0; z < Z; z++){
            bucket[z].data.ind = -1;
            bucket[z].data.keyword = "dummydata";
            bucket[z].data.op = OP::INS;
            bucket[z].data.pth = -1;
        }
        WriteBucket(i, bucket, kSO);
    }
}

SORAM::~SORAM() {
    AES::Cleanup();
}

block SORAM::SerialiseBucket(Bucket bucket) {
    block buffer;

    for (int z = 0; z < Z; z++) {
        Block b = bucket[z];
        string plain = to_string(b.data.ind) + ":" + to_string(b.data.op) + ":" + to_string(b.data.pth) + ":" + b.data.keyword;

        uint8_t* p = reinterpret_cast<uint8_t*>(&plain[0]);
        auto r = vector<byte_t>(p, p + sizeof(plain));

        buffer.insert(buffer.end(), r.begin(), r.end());
    }
    assert(buffer.size() == Z * (blockSize));

    return buffer;
}

Bucket SORAM::DeserialiseBucket(block buffer) {
    assert(buffer.size() == Z * (blockSize));

    Bucket bucket;
    block b;
    for (int z = 0; z < Z; z++) {
        string plain;
        b.assign(buffer.begin(), buffer.begin() + blockSize);
        for(auto i = b.begin(); i != b.end(); i++){
            plain += *i;
        }
        bucket[z].data.ind = stoi(plain.substr(0, plain.find(":")));
        plain.erase(0, plain.find(":") + sizeof(char));

        bucket[z].data.op = (OP)stoi(plain.substr(0, plain.find(":")));
        plain.erase(0, plain.find(":") + sizeof(char));

        bucket[z].data.pth = stoi(plain.substr(0, plain.find(":")));
        plain.erase(0, plain.find(":") + sizeof(char));

        bucket[z].data.keyword = &plain[0];

        buffer.erase(buffer.begin(), buffer.begin() + blockSize);
    }

    return bucket;
}

int SORAM::getNumberOfLeaves(){
    return nOfLeaves;
}

int SORAM::getFirstLeaf(){
    return firstLeaf;
}

block SORAM::EncryptNode(Bucket bucket, bytes<Key> kSO){
    block b = SerialiseBucket(bucket);
    block ciphertext = AES::Encrypt(kSO, b, clen_size, plaintext_size);
    return ciphertext;
}

void SORAM::WriteBucket(int index, Bucket bucket, bytes<Key> kSO){
    block b = SerialiseBucket(bucket);
    block ciphertext = AES::Encrypt(kSO, b, clen_size, plaintext_size);
    nodes[index].value = ciphertext;
}

Bucket SORAM::ReadBucket(block ciphertext, bytes<Key> kSO) {
    block buffer = AES::Decrypt(kSO, ciphertext, clen_size);
    Bucket bucket = DeserialiseBucket(buffer);
    return bucket;
}

block SORAM::ReadBlock(int n){
    return nodes[n].value;
}

void SORAM::setNode(int i, block value){
    nodes[i].value = value;
}

int SORAM::RandomPath() {
    int val = dis(mt);
    return val;
}
