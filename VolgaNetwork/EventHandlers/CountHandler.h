#pragma once
#include "IEventHandler.h"

class CountHandler : public IEventHandler
{
public:
    void HandleEvent(Node* Sender, Node* Receiver) override;
};
