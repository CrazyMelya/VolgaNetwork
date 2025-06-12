#pragma once
#include "IEventHandler.h"

class SumHandler : public IEventHandler
{
public:
    void HandleEvent(Node* sender, Node* receiver) override;
};
