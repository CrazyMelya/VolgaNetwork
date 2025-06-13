#include "Network.h"

#include <iostream>
#include <string>

#include "Node.h"
#include "EventHandlers/CountHandler.h"
#include "EventHandlers/SumHandler.h"

Network::Network()
{
    handlerFactory_.RegisterHandler("Count", [] { return std::make_unique<CountHandler>(); });
    handlerFactory_.RegisterHandler("Sum", [] { return std::make_unique<SumHandler>(); });

    for (int i = 0; i < 5; ++i) {
        AddNode();
    }
    
    for (auto& node : nodes_) {
        for (auto& other : nodes_) {
            if (node.get() != other.get() && dist_(rng_) < 0.4) {
                node->Subscribe(other.get(), handlerFactory_.CreateRandomHandler());
            }
        }
    }
}

void Network::tick()
{
    for (auto& node : nodes_) {
        node->UpdateNeighborsCache();
        std::cout << step_ << " " << node->GetName() << '\n';
    }
    step_++;
}

void Network::run(int steps)
{
    for (int i = 0; i < steps; ++i) {
        tick();
    }
}

Node* Network::AddNode()
{
    std::string name = "Node_" + std::to_string(nodes_.size());
    nodes_.push_back(std::make_unique<Node>(name, this));
    return nodes_.back().get();
}