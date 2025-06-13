#include "EventHandlerFactory.h"

void EventHandlerFactory::RegisterHandler(const std::string& name, CreateHandlerFn factoryFn)
{
    registry_[name] = std::move(factoryFn);
}

std::unique_ptr<IEventHandler> EventHandlerFactory::CreateRandomHandler()
{
    std::uniform_int_distribution<size_t> dist (0, registry_.size() - 1);
    if (registry_.empty())
        return nullptr;

    size_t index = dist(rng_);
    auto it = registry_.begin();
    std::advance(it, index);
    return it->second();
}
