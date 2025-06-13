#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "EventHandlers/IEventHandler.h"


class Network;

struct Event {
    class Node* sender;
    int value;
};

class Node
{
public:
    explicit Node(std::string name, Network* network);
    const std::vector<int>* GetData(const Node* sender) const;
    std::string GetName() const { return name_; }
    void Subscribe(Node* target, std::unique_ptr<IEventHandler> handler);
    void Unsubscribe(Node* target);
    void UpdateCache();
    void HandleEvent(Event event);

    void DoSubscribe();
    void DoUnsubscribe();
    void DoSendEvent();
    void DoCreateNode();

    bool HasSubscriptions() const { return !subscriptions_.empty(); }
    bool HasSubscribers() const { return !subscribers_.empty(); }

private:
    std::string name_;
    std::unordered_map<Node*, std::unique_ptr<IEventHandler>> subscriptions_;
    std::unordered_set<Node*> subscribers_;
    std::unordered_map<Node*, std::vector<int>> received_data_;

    std::unordered_set<Node*> cached_neighbors_;
    std::unordered_set<Node*> cached_subscribers_;

    Network* network_ = nullptr;

    Node* FindSubscriptionTarget() const;
    Node* FindUnsubscriptionTarget() const;
};
