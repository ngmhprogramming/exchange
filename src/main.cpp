#include <exchange.hpp>
#include <iostream>

int main() {
    std::cout << "Order struct has size " << sizeof(exchange::Order)
              << " bytes\n";

    exchange::Order order{1001, 1686230400, 150.25, 50, exchange::Side::Buy};

    std::cout << "Order with ID " << order.id << " initialised\n";

    std::cout << "Order created: " << order << "\n";
}