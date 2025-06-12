#pragma once

class Node;

class IEventHandler
{
public:
    virtual ~IEventHandler() = default;
    virtual void HandleEvent(Node* sender, Node* receiver) = 0;
};
