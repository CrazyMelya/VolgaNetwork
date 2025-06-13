#include "EventHandlerFactory.h"

void EventHandlerFactory::RegisterHandler(const std::string& Name, CreateHandlerFn FactoryFn)
{
    registry_[Name] = std::move(FactoryFn);
}

std::unique_ptr<IEventHandler> EventHandlerFactory::CreateRandomHandler()
{
    std::uniform_int_distribution<size_t> dist (0, registry_.size() - 1);
    if (registry_.empty())
        return nullptr;

    const size_t index = dist(rng_);
    auto it = registry_.begin();
    std::advance(it, index);
    return it->second();
}
