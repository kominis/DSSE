#include "mara/Client.hpp"

using namespace std;

int main(int argc, char** argv) {
    // N is the number of nodes
    // and max is the total number of Data
    int N, max;
    // give N and max from command line
    if(argc == 3){
        max = atoi(argv[1]);
        N = atoi(argv[2]);
    }else{
        // or set default values
        max = 100;
        N = 131072;
    }
    // how many data to delete
    int delperc = 50;
    Server server;
    Client client(&server, N);

    // Initialize Data
    map<string, dataBlocks> Data;

    string ar[22] = {"test1", "test2", "test3", "test23", "test5",
     "test6", "test7", "test8", "test9", "test10", "test11", "test12",
     "test13", "test14", "test15", "test16", "test17", "test18", "test19",
     "test20", "test21", "test22"};
    srand(time(0));
    std::random_device rd;
    std::mt19937 mt(rd());
    auto dis = std::uniform_int_distribution<int>{0, 21};
    auto dis1 = std::uniform_int_distribution<int>{0, 999999999};

    // keyword to search for and delete
    for(int i=1;i<=delperc;i++){
        dataBlocks t;
        t.ind = i;
        t.op = static_cast<OP>(0);
        t.keyword = "test4";
        t.pth = -1;
        Data[to_string(t.op) + to_string(t.ind) + t.keyword] = t;
    }

    for(int i=0;i<max-delperc;i++){
        dataBlocks t;
        t.ind = dis1(mt);
        t.op = static_cast<OP>((rand() % 2));
        t.keyword = ar[dis(mt)];
        t.pth = -1;
        Data[to_string(t.op) + to_string(t.ind) + t.keyword] = t;
    }

    cout << "Data size = " << Data.size() << endl;

    cout << "Start Update..." << endl;
    Auxiliary::startTimer(1);
    client.update(Data);
    cout << "Update time = " << Auxiliary::stopTimer(1) << " milliseconds" << endl;

    cout << "Start Search..." << endl;
    Auxiliary::startTimer(2);
    client.search("test4");
    cout << "Search time = " << Auxiliary::stopTimer(2) << " milliseconds" << endl;
    return 0;
}
