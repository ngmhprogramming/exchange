#include <exchange.hpp>
#include <iostream>

int main() {
    std::cout << "Order struct has size " << sizeof(exchange::Order)
              << " bytes\n";

    exchange::Order order{1001, 1686230400, 150.25, 50, exchange::Side::Buy};

    std::cout << "Order with ID " << order.id << " initialised\n";

    std::cout << "Order created: " << order << "\n";

    exchange::OrderBook book;

    exchange::Order ask1{1, 1001, 150.00, 100, exchange::Side::Sell};
    book.addOrder(ask1);
    exchange::Order buy1{2, 1002, 150.00, 100, exchange::Side::Buy};
    book.addOrder(buy1); // Execute a trade for 100
    std::cout << book;

    exchange::Order ask2{3, 1003, 200.00, 100, exchange::Side::Sell};
    book.addOrder(ask2);
    exchange::Order buy2{4, 1004, 200.00, 40, exchange::Side::Buy};
    book.addOrder(buy2); // Execute a trade for 40, leaving 60
    std::cout << book;

    exchange::Order buy3{5, 1005, 190.00, 50, exchange::Side::Buy};
    book.addOrder(buy3); // No trade should be executed
    std::cout << book;

    exchange::OrderBook ladder_book;
    for (int i = 0; i < 50; i++) {
        double price = i;
        uint32_t qty = i * 100;
        exchange::Side side =
            i <= 25 ? exchange::Side::Buy : exchange::Side::Sell;
        exchange::Order o{static_cast<uint64_t>(i),
                          static_cast<uint64_t>(1000 + i), price, qty, side};
        ladder_book.addOrder(o);
    }
    std::cout << "\n" << ladder_book << "\n";
}