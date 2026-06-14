#include <exchange.hpp>
#include <iostream>
#include <vector>

int main() {
    exchange::OrderBook ladder_book;
    std::vector<exchange::TradeReport> res;
    for (int i = 0; i < 50; i++) {
        uint64_t price = i * 10000;
        uint32_t qty = i * 100;
        exchange::Side side =
            i <= 25 ? exchange::Side::Buy : exchange::Side::Sell;
        exchange::Order o{static_cast<uint64_t>(i),
                          static_cast<uint64_t>(1000 + i), price, qty, side};
        ladder_book.addOrder(o, res);
    }
    std::cout << "\n" << ladder_book << "\n";
}