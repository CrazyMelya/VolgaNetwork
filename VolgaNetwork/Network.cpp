#include "Network.h"

#include <iostream>
#include <string>

#include "Node.h"
#include "EventHandlers/CountHandler.h"
#include "EventHandlers/SumHandler.h"

Network::Network() : dist_(0.0, 1.0) {
    // Создаём 5 узлов
    for (int i = 0; i < 5; ++i) {
        nodes_.push_back(std::make_unique<Node>("Node_" + std::to_string(i)));
    }

    // Устанавливаем случайные подписки
    for (auto& node : nodes_) {
        for (auto& other : nodes_) {
            if (node.get() != other.get() && dist_(rng_) < 0.4) {
                if (dist_(rng_) < 0.5) {
                    node->Subscribe(other.get(), std::make_unique<SumHandler>());
                } else {
                    node->Subscribe(other.get(), std::make_unique<CountHandler>());
                }
            }
        }
    }
}

void Network::step() {
    // Генерация событий с вероятностью 40%
    for (auto& node : nodes_) {
        if (dist_(rng_) < 0.4) {
            int value = rng_() % 100;
            for (Node* subscriber : node->subscribers) {
                subscriber->receiveEvent(node.get(), value);
            }
        }
    }

    // Обработка событий
    for (auto& node : nodes_) {
        node->handleEvents();
    }

    // TODO: можно добавить удаление изолированных узлов
}

void Network::run(int steps) {
    for (int i = 0; i < steps; ++i) {
        cout << "\n=== Step " << i + 1 << " ===\n";
        step();
    }
}