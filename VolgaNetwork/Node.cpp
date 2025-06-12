#include "Node.h"

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

Node* Node::FindSubscriptionTarget() const {
    std::unordered_set<Node*> neighbors;

    // Соседи: те, кто подписан на меня
    for (Node* sub : subscribers_) {
        neighbors.insert(sub);
    }

    // Соседи: те, на кого я подписан
    for (const auto& pair : subscriptions_) {
        neighbors.insert(pair.first);
    }

    // Кандидаты: соседи соседей, исключая себя и уже подписанных
    std::unordered_set<Node*> candidates;

    for (Node* neighbor : neighbors) {
        // Те, кто подписан на соседа
        for (Node* n_sub : neighbor->subscribers_) {
            if (n_sub != this && subscriptions_.find(n_sub) == subscriptions_.end()) {
                candidates.insert(n_sub);
            }
        }

        // Те, на кого подписан сосед
        for (const auto& pair : neighbor->subscriptions_) {
            Node* n_target = pair.first;
            if (n_target != this && subscriptions_.find(n_target) == subscriptions_.end()) {
                candidates.insert(n_target);
            }
        }
    }

    // Выбираем случайного кандидата, если есть
    if (!candidates.empty()) {
        auto it = candidates.begin();
        std::advance(it, rand() % candidates.size());
        return *it;
    }

    return nullptr;
}
