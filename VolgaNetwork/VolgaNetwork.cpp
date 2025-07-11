
#include <iostream>
#include "Network.h"

size_t get_initial_nodes_count()
{
    size_t initial_nodes_count;
    while (true) {
        std::cout << "Enter the number of initial nodes (positive integer): ";
        if (std::cin >> initial_nodes_count && initial_nodes_count > 0)
            return initial_nodes_count;

        std::cout << "Please enter a positive integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double get_initial_subscription_probability()
{
    double initial_subscription_probability;
    while (true) {
        std::cout << "Enter initial subscription probability (0 to 1): ";
        if (std::cin >> initial_subscription_probability && initial_subscription_probability >= 0.0 && initial_subscription_probability <= 1.0)
            return initial_subscription_probability;

        std::cout << "Please enter a probability between 0 and 1.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::vector<double> get_action_weights()
{
    const std::vector<std::string> action_names = {
        "Subscribe",
        "Unsubscribe",
        "CreateNode",
        "SendEvent",
        "DoNothing"
    };

    std::vector<double> user_weights(action_names.size());

    for (size_t i = 0; i < user_weights.size(); ++i) {
        double w;
        while (true) {
            std::cout << "Enter weight for action \"" << action_names[i] << "\" (non-negative number): ";
            std::cin >> w;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Error: input must be a number. Please try again.\n";
                continue;
            }
            if (w < 0) {
                std::cout << "Error: weight cannot be negative. Please try again.\n";
                continue;
            }
            break;
        }
        user_weights[i] = w;
    }
    return user_weights;
}

int main(int argc, char* argv[])
{
    const size_t initial_nodes_count = get_initial_nodes_count();
    const double initial_subscription_probability = get_initial_subscription_probability();
    const std::vector<double> user_weights = get_action_weights();

    Network net;
    net.Init(initial_nodes_count, initial_subscription_probability, user_weights);
    net.Run();

    return 0;
}
