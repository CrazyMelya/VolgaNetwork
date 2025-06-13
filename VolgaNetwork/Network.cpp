#include "Network.h"

#include <iostream>
#include <string>

#include "Node.h"
#include "EventHandlers/CountHandler.h"
#include "EventHandlers/SumHandler.h"

Network::Network() : rng_(random_device()()), dist_(0.0, 1.0) {
    for (int i = 0; i < 5; ++i) {
        nodes_.push_back(std::make_unique<Node>("Node_" + std::to_string(i)));
    }
    
    for (auto& node : nodes_) {
        for (auto& other : nodes_) {
            if (node.get() != other.get() && dist_(rng_) < 0.4) {
                if (dist_(rng_) < 0.5) {
                    node->Subscribe(other.get(), std::make_unique<SumHandler>());
                } else {
                    node->Subscribe(other.get(), std::make_unique<CountHandler>());
                }
            }
        }
    }
}

void Network::step() {
    for (auto& node : nodes_)
    {
        cout << step_ << " " << node->GetName() << '\n';
    }
    step_++;
}

void Network::run(int steps) {
    for (int i = 0; i < steps; ++i) {
        step();
    }
}