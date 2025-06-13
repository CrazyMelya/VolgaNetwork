#pragma once
#include "IEventHandler.h"

class CountHandler : public IEventHandler
{
public:
    void HandleEvent(Node* sender, Node* receiver) override;
};
