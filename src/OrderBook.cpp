#include <exchange/OrderBook.hpp>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <vector>

namespace exchange {

void OrderBook::matchOrder(Order &current_order, auto &other_side,
                           auto can_match, std::vector<TradeReport> &res) {
    while (!other_side.empty() && current_order.quantity > 0) {
        // Check if we can fill the order
        const auto &best_price = other_side.begin()->first;
        auto &best_level = other_side.begin()->second;
        if (!can_match(current_order.price, best_price)) {
            break;
        }

        // Fill the order and report
        auto &best_order = best_level.front();
        uint32_t filled_quantity =
            std::min(current_order.quantity, best_order.quantity);
        res.emplace_back(best_order.id, current_order.id,
                         current_order.timestamp, best_order.price,
                         filled_quantity);
        current_order.quantity -= filled_quantity;
        best_order.quantity -= filled_quantity;

        // Update order book
        if (best_order.quantity == 0) {
            best_level.pop_front();
        }
        if (best_level.empty()) {
            other_side.erase(other_side.begin());
        }
    }
}

void OrderBook::addOrder(const Order &order, std::vector<TradeReport> &res) {
    Order current_order = order;
    res.clear();
    if (order.side == Side::Buy) {
        matchOrder(current_order, asks, std::greater_equal<double>{}, res);
        if (current_order.quantity > 0) {
            bids[order.price].push_back(current_order);
            std::cout << "Added bid " << current_order << "\n";
        }
    } else {
        matchOrder(current_order, asks, std::less_equal<double>{}, res);
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