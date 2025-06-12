#pragma once
#include <memory>
#include <random>
#include <vector>

using namespace std;

class Node;

class Network
{
    vector<unique_ptr<Node>> nodes_;
    mt19937 rng_{static_cast<unsigned>(std::time(nullptr))};
    std::uniform_real_distribution<> dist_;

public:
    Network();

    void step();

    void run(int steps);
};
