#include "CountHandler.h"

#include <iostream>
#include <vector>
#include "../Node.h"

void CountHandler::HandleEvent(Node* Sender, Node* Receiver)
{
    const std::vector<int>* Data = Receiver->GetData(Sender);
    if (Data) {
        std::cout << Sender->GetName() << " -> " << Receiver->GetName() << ": N = " << Data->size() << '\n';
    }
}
