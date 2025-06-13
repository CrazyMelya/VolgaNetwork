#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "EventHandlers/IEventHandler.h"

using namespace std;

struct Probabilities {
    double createEvent = 0.2;
    double subscribe = 0.2;
    double unsubscribe = 0.2;
    double spawnNode = 0.2;
    double doNothing = 0.2;
};

struct Event {
    class Node* sender;
    int value;
};

class Node
{
public:
    explicit Node(std::string name);
    const vector<int>* GetData(const Node* sender) const;
    string GetName() const { return name_; }
    void Subscribe(Node* target, unique_ptr<IEventHandler> handler);
    void Unsubscribe(Node* target);
    void UpdateNeighborsCache();

private:
    string name_;
    unordered_map<Node*, unique_ptr<IEventHandler>> subscriptions_;
    unordered_set<Node*> subscribers_;
    unordered_map<Node*, std::vector<int>> received_data_;
    unordered_set<Node*> cached_neighbors_;

    Node* FindSubscriptionTarget() const;
};
