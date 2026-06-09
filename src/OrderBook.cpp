#include <exchange/OrderBook.hpp>
#include <iomanip>
#include <iostream>
#include <ranges>

namespace exchange {

void OrderBook::addOrder(const Order &order) {
    Order current_order = order;
    if (order.side == Side::Buy) {
        // Matching logic
        while (!asks.empty() && current_order.quantity > 0 &&
               current_order.price >= asks.begin()->first) {
            auto &best_asks = asks.begin()->second;
            auto &best_ask = best_asks.front();
            uint32_t filled_quantity =
                std::min(current_order.quantity, best_ask.quantity);
            std::cout << "Executing trade " << filled_quantity << " @ "
                      << best_ask.price << "\n";
            current_order.quantity -= filled_quantity;
            best_ask.quantity -= filled_quantity;
            if (best_ask.quantity == 0) {
                best_asks.pop_front();
            }
            if (best_asks.empty()) {
                asks.erase(asks.begin());
            }
        }
        if (current_order.quantity > 0) {
            bids[order.price].push_back(current_order);
            std::cout << "Added bid " << current_order << "\n";
        }
    } else {
        // Matching logic
        while (!bids.empty() && current_order.quantity > 0 &&
               current_order.price <= bids.begin()->first) {
            auto &best_bids = bids.begin()->second;
            auto &best_bid = best_bids.front();
            uint32_t filled_quantity =
                std::min(current_order.quantity, best_bid.quantity);
            std::cout << "Executing trade " << filled_quantity << " @ "
                      << best_bid.price << "\n";
            current_order.quantity -= filled_quantity;
            best_bid.quantity -= filled_quantity;
            if (best_bid.quantity == 0) {
                best_bids.pop_front();
            }
            if (best_bids.empty()) {
                bids.erase(bids.begin());
            }
        }
        if (current_order.quantity > 0) {
            asks[order.price].push_back(current_order);
            std::cout << "Added ask " << current_order << "\n";
        }
    }
}

std::ostream &operator<<(std::ostream &os, const OrderBook &book) {
    const std::string GREEN = "\033[32m";
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";

    os << std::right << std::setw(10) << "Bid"
       << " | " << std::setw(8) << "Price"
       << " | " << std::left << std::setw(10) << "Ask"
       << "\n";
    os << "-----------------------------------\n";
    for (const auto &[price, orders] : book.asks | std::views::reverse) {
        uint32_t volume = 0;
        for (const auto &order : orders) {
            volume += order.quantity;
        }
        os << std::right << GREEN << std::setw(10) << "" << RESET << " | ";
        os << std::fixed << std::setprecision(2) << std::setw(8) << price
           << " | ";
        os << std::left << RED << std::setw(10)
           << (volume > 0 ? std::to_string(volume) : "") << RESET << "\n";
    }
    for (const auto &[price, orders] : book.bids) {
        uint32_t volume = 0;
        for (const auto &order : orders) {
            volume += order.quantity;
        }
        os << std::right << GREEN << std::setw(10)
           << (volume > 0 ? std::to_string(volume) : "") << RESET << " | ";
        os << std::fixed << std::setprecision(2) << std::setw(8) << price
           << " | ";
        os << std::left << RED << std::setw(10) << "" << RESET << "\n";
    }
    return os;
}

} // namespace exchange