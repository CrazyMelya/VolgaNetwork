#pragma once
#include "IEventHandler.h"

class SumHandler : public IEventHandler
{
public:
    void HandleEvent(Node* Sender, Node* Receiver) override;
};
