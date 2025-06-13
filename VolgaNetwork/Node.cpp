#include "Node.h"

#include <algorithm>
#include <random>
#include <utility>

Node::Node(std::string name) : name_(std::move(name)) {}

const std::vector<int>* Node::GetData(const Node* sender) const
{
    const auto it = received_data_.find(const_cast<Node*>(sender));
    if (it != received_data_.end()) {
        return &it->second;
    }
    return nullptr;
}

void Node::Subscribe(Node* target, unique_ptr<IEventHandler> handler)
{
    if (!target || !handler || target == this) return;
    subscriptions_[target] = std::move(handler);
    target->subscribers_.insert(this);
}

void Node::Unsubscribe(Node* target)
{
    if (!target || target == this) return;

    auto it = subscriptions_.find(target);
    if (it != subscriptions_.end()) {
        subscriptions_.erase(it);
        target->subscriptions_.erase(this);
    }
}

void Node::UpdateNeighborsCache()
{
    cached_neighbors_.clear();
    cached_neighbors_.insert(subscribers_.begin(), subscribers_.end());
    for (const auto& pair : subscriptions_)
        cached_neighbors_.insert(pair.first);
}

Node* Node::FindSubscriptionTarget() const
{
    unordered_set<Node*> candidates;
    for (const Node* firstNeighbor : cached_neighbors_) {
        for (Node* secondNeighbor : firstNeighbor->cached_neighbors_) {
            if (secondNeighbor != this && subscriptions_.find(secondNeighbor) == subscriptions_.end()) {
                candidates.insert(secondNeighbor);
            }
        }
    }

    if (!candidates.empty())
    {
        static thread_local mt19937 rng(random_device{}());
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    
        auto it = candidates.begin();
        std::advance(it, dist(rng));
        return *it;
    }
    
    return nullptr;
}
