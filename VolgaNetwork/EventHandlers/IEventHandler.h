#pragma once

class Node;

class IEventHandler
{
public:
    virtual ~IEventHandler() = default;
    virtual void HandleEvent(Node* Sender, Node* Receiver) = 0;
};
