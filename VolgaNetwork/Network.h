#pragma once
#include <memory>
#include <random>
#include <vector>
#include"Node.h"

using namespace std;

class Network
{
    vector<unique_ptr<Node>> nodes_;
    mt19937 rng_;
    std::uniform_real_distribution<> dist_;
    int step_ = 0;

public:
    Network();

    void step();

    void run(int steps);
};
