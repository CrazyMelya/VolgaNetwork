#pragma once
#include <memory>
#include <random>
#include <vector>
#include"Node.h"
#include "EventHandlers/EventHandlerFactory.h"

enum class NodeAction {
    Subscribe,
    Unsubscribe,
    CreateNode,
    CreateEvent,
    DoNothing
};

class Network
{
    std::vector<std::unique_ptr<Node>> nodes_;
    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<double> dist_{0.0, 1.0 };
    int step_ = 0;
    EventHandlerFactory handlerFactory_;

public:
    Network();

    EventHandlerFactory& GetHandlerFactory() { return handlerFactory_; }
    void tick();
    void run(int steps);
    Node* AddNode();
};
