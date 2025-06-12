#include "CountHandler.h"

#include <iostream>
#include <vector>
#include "../Node.h"

void CountHandler::HandleEvent(Node* sender, Node* receiver)
{
    const vector<int>* data = receiver->GetData(sender);
    if (data) {
        cout << sender->GetName() << " -> " << receiver->GetName() << ": N = " << data->size() << '\n';
    }
}
