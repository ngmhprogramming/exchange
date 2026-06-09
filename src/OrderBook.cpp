#include <exchange/OrderBook.hpp>

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

} // namespace exchange