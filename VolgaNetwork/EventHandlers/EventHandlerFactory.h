#pragma once

#include <map>
#include <string>
#include <functional>
#include <memory>
#include <random>

#include "IEventHandler.h"


class EventHandlerFactory
{
    using CreateHandlerFn = std::function<std::unique_ptr<IEventHandler>()>;
    
    std::map<std::string, CreateHandlerFn> registry_;
    std::mt19937 rng_{std::random_device{}()};

public:
    void RegisterHandler(const std::string& name, CreateHandlerFn factoryFn);

    std::unique_ptr<IEventHandler> CreateRandomHandler();
};
