#include "Node.h"

#include <algorithm>
#include <random>
#include <utility>

#include "Network.h"
#include "EventHandlers/CountHandler.h"

Node::Node(std::string name, Network* network) : name_(std::move(name)), network_(network) {}

const std::vector<int>* Node::GetData(const Node* sender) const
{
    const auto it = received_data_.find(const_cast<Node*>(sender));
    if (it != received_data_.end()) {
        return &it->second;
    }
    return nullptr;
}

void Node::Subscribe(Node* target, std::unique_ptr<IEventHandler> handler)
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

void Node::HandleEvent(const Event event)
{
    if (event.sender)
    {
        auto& vec = received_data_[event.sender];
        vec.push_back(event.value);
    
        auto it = subscriptions_.find(event.sender);
        if (it != subscriptions_.end() && it->second) {
            it->second->HandleEvent(event.sender, this);
        }
    }
}

Node* Node::FindSubscriptionTarget() const
{
    std::unordered_set<Node*> candidates;
    for (const Node* firstNeighbor : cached_neighbors_) {
        for (Node* secondNeighbor : firstNeighbor->cached_neighbors_) {
            if (secondNeighbor != this && subscriptions_.find(secondNeighbor) == subscriptions_.end()) {
                candidates.insert(secondNeighbor);
            }
        }
    }

    if (!candidates.empty())
    {
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    
        auto it = candidates.begin();
        std::advance(it, dist(rng));
        return *it;
    }
    
    return nullptr;
}

Node* Node::FindUnsubscriptionTarget() const
{
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, cached_neighbors_.size() - 1);
    auto it = cached_neighbors_.begin();
    std::advance(it, dist(rng));
    return *it;
}

void Node::DoSubscribe()
{
    Node* SubscriptionTarget = FindSubscriptionTarget();
    if (SubscriptionTarget)
        Subscribe(SubscriptionTarget, network_->GetHandlerFactory().CreateRandomHandler());
}

void Node::DoUnsubscribe()
{
    Node* UsubscriptionTarget = FindUnsubscriptionTarget();
    if (UsubscriptionTarget)
        Unsubscribe(UsubscriptionTarget);
}

void Node::DoSendEvent()
{
    Event event;
    event.sender = this;
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 10);
    event.value = dist(rng);
    for (const auto subscriber : subscribers_) {
        subscriber->HandleEvent(event);
    }
}

void Node::DoCreateNode()
{
    if (!network_) return;
    
    Node* newNode = network_->AddNode();
    
    if (newNode) {
        Subscribe(newNode, network_->GetHandlerFactory().CreateRandomHandler());
    }
}