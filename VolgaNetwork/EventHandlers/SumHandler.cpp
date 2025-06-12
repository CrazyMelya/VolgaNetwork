#include "SumHandler.h"

#include <iostream>
#include <numeric>
#include <ostream>
#include <vector>
#include "../Node.h"

void SumHandler::HandleEvent(Node* sender, Node* receiver)
{
    const std::vector<int>* data = receiver->GetData(sender);
    if (data && !data->empty()) {
        int sum = std::accumulate(data->begin(), data->end(), 0);
        std::cout << sender->GetName() << " -> " << receiver->GetName() << ": S = " << sum << '\n';
    }
}
