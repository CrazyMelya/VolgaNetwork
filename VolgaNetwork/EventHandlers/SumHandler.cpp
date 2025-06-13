#include "SumHandler.h"

#include <iostream>
#include <numeric>
#include <ostream>
#include <vector>
#include "../Node.h"

void SumHandler::HandleEvent(Node* Sender, Node* Receiver)
{
    const std::vector<int>* data = Receiver->GetData(Sender);
    if (data && !data->empty()) {
        int sum = std::accumulate(data->begin(), data->end(), 0);
        std::cout << Sender->GetName() << " -> " << Receiver->GetName() << ": S = " << sum << '\n';
    }
}
