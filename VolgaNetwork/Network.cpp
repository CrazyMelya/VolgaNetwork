#include "Network.h"

#if _WIN32 || _WIN64
#include <conio.h>
#endif 
#include <string>
#include "Node.h"
#include "EventHandlers/CountHandler.h"
#include "EventHandlers/SumHandler.h"

Network::Network()
{
    handlerFactory_.RegisterHandler("Count", [] { return std::make_unique<CountHandler>(); });
    handlerFactory_.RegisterHandler("Sum", [] { return std::make_unique<SumHandler>(); });
}

void Network::Tick()
{
    for (const auto& node : nodes_) {
        node->UpdateCache();
    }
    for (auto& node : nodes_) {
        PerformAction(node.get());
    }
    UpdateNodes();
}

void Network::Run()
{
    while (!nodes_.empty()){
        Tick();
#if _WIN32 || _WIN64
        if (_kbhit())
            break;
#endif
    }
}

Node* Network::AddNode()
{
    std::string name = "Node_" + std::to_string(current_node_num_);
    pending_nodes_.push_back(std::make_unique<Node>(name, this));
    current_node_num_++;
    return pending_nodes_.back().get();
}

void Network::Init(size_t initial_node_count, double initial_subscription_probability, std::vector<double> action_weights)
{
    action_weights_ = std::move(action_weights);
    action_distribution_ = std::discrete_distribution<size_t>(action_weights_.begin(), action_weights_.end());
    for (size_t i = 0; i < initial_node_count; ++i)
        AddNode();

    for (auto& node : pending_nodes_)
        nodes_.push_back(std::move(node));
    pending_nodes_.clear();

    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    for (auto& node : nodes_) {
        for (auto& other : nodes_) {
            if (node.get() != other.get() && dist_(rng_) < initial_subscription_probability) {
                node->Subscribe(other.get(), handlerFactory_.CreateRandomHandler());
            }
        }
    }
}

void Network::PerformAction(Node* node)
{
    size_t action_index = action_distribution_(rng_);

    switch (action_index) {
    case 0:
        node->DoSubscribe();
        break;
    case 1:
        node->DoUnsubscribe();
        break;
    case 2:
        node->DoCreateNode();
        break;
    case 3:
        node->DoSendEvent();
        break;
    default:
        break;
    }
}

void Network::UpdateNodes()
{
    // std::cout << "Updating nodes\n";
    nodes_.erase(std::remove_if(nodes_.begin(), nodes_.end(),
        [](const std::unique_ptr<Node>& node_ptr) {
            bool remove = !node_ptr->HasSubscriptions() && !node_ptr->HasSubscribers();
            // if (remove) {
            //     std::cout << "Removing node " << node_ptr->GetName() << " due to no subs and no subscribers\n";
            // }
            return remove;
        }),
        nodes_.end());
    for (auto& new_node : pending_nodes_)
        nodes_.push_back(std::move(new_node));
    pending_nodes_.clear();
}
