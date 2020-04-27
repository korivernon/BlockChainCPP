#include<ctime>
#include<vector>
#include<iostream>
#include<string>

using namespace std;

class TransactionData{
public:
    double amt;
    string senderKey;
    string recKey;
    time_t timestamp;
};

class Block{
private:
    int index;
    size_t prevHash;
    size_t blockHash;
    size_t generateHash();
public:
    Block(int i, TransactionData d, size_t previousHash);
    // Block(int i, TransactionData d, size_t previousHash) : index(i), data(d), prevHash(previousHash) {}
    size_t getHash(){return blockHash;}
    size_t getPreviousHash(){return prevHash;}
    TransactionData data; // Transaction Data Member
    bool isHashValid();
};

Block::Block(int i, TransactionData d, size_t previousHash){
    index = i;
    data = d;
    prevHash = previousHash;
    blockHash = generateHash();
}
size_t Block::generateHash(){
    hash<string> fhash;
    hash<size_t> shash;
    hash<size_t> finHash;
    string toHash = to_string(data.amt)+data.recKey+data.senderKey+to_string(data.timestamp);
    // have the string contain the amount of data, the receiver key, sender key, and timestamp
    return finHash(fhash(toHash)+shash(prevHash));
}
bool Block::isHashValid(){
    return generateHash() == blockHash; // check whether the block has been manipulated
}
class BlockChain{
private:
    Block createOrigin();
public:
    vector<Block> blockChain; // vector of blocks
    BlockChain();
    void addBlck(TransactionData data);
    bool isChainValid();
    Block *getLatestBlock();
};

BlockChain::BlockChain(){
    Block origin = createOrigin();
    blockChain.push_back(origin);
}
Block BlockChain::createOrigin(){
    time_t current; // current time 
    TransactionData d;
    d.amt = 0;
    d.recKey = "None";
    d.senderKey = "None";
    d.timestamp = time(&current);

    hash<int> fhash;
    Block origin(0, d, fhash(0));
    return origin;
}

Block *BlockChain::getLatestBlock(){
    return &blockChain.back();
}

void BlockChain::addBlck(TransactionData d){
    int index = (int)blockChain.size() -1;
    Block newBlock(index, d, getLatestBlock()->getHash());
}

bool BlockChain::isChainValid(){
    vector<Block>::iterator it;
    int chainLen = (int)blockChain.size();

    for(it = blockChain.begin(); it != blockChain.end(); it++){

        Block currentBlock = *it;
        if( !currentBlock.isHashValid()) return false; //someone tried to alter transaction data
        else{
            if (chainLen < 1){
                Block previousBlock = *(it-1);
                if (currentBlock.getPreviousHash() != previousBlock.getHash()) return false; //someone tried to alter transaction data
            }
        }
    }
    return true;
}
int main(){
    BlockChain AlkebulanCoin;
    TransactionData data1;
    time_t data1Time;
    data1.amt = 1.5;
    data1.recKey = "Kori";
    data1.senderKey = "Return me to Africa";
    data1.timestamp = time(&data1Time);

    AlkebulanCoin.addBlck(data1);
    cout << "Is chain valid? " << AlkebulanCoin.isChainValid() << endl;
}