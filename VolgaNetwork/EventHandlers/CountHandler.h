#pragma once
#include "IEventHandler.h"

using namespace std;

class CountHandler : public IEventHandler
{
public:
    void HandleEvent(Node* sender, Node* receiver) override;
};
