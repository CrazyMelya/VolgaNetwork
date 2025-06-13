#include "CountHandler.h"

#include <iostream>
#include <vector>
#include "../Node.h"

void CountHandler::HandleEvent(Node* sender, Node* receiver)
{
    const std::vector<int>* data = receiver->GetData(sender);
    if (data) {
        std::cout << sender->GetName() << " -> " << receiver->GetName() << ": N = " << data->size() << '\n';
    }
}
