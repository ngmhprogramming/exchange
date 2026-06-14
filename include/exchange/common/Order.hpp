#pragma once
#include <cstdint>
#include <ostream>

namespace exchange {

enum class Side : uint8_t { Buy, Sell };

struct Order {
    uint64_t id;
    uint64_t timestamp;
    uint64_t price; // multiplier of 10000
    uint32_t quantity;
    Side side;

    friend std::ostream &operator<<(std::ostream &os, const Order &order) {
        return os << "Order ("
                  << "ID: " << order.id << ", Timestamp: " << order.timestamp
                  << ", Price: " << order.price
                  << ", Quantity: " << order.quantity << ", Side: "
                  << (order.side == exchange::Side::Buy ? "Buy" : "Sell")
                  << ")";
    }
};

} // namespace exchange