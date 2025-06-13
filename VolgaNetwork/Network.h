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
    std::vector<std::unique_ptr<Node>> pending_nodes_;
    std::mt19937 rng_{std::random_device{}()};
    std::uniform_real_distribution<double> dist_{0.0, 1.0 };
    std::vector<double> action_weights_;
    std::discrete_distribution<size_t> action_distribution_;
    EventHandlerFactory handlerFactory_;
    size_t current_node_num_ = 0;;

public:
    Network();

    EventHandlerFactory& GetHandlerFactory() { return handlerFactory_; }
    void run();
    Node* AddNode();
    void Init(size_t initial_node_count, double initial_subscription_probability, std::vector<double> action_weights = {0.2, 0.2, 0.2, 0.2, 0.2});

private:
    void tick();
    void PerformAction(Node* node);
    void UpdateNodes();
};
