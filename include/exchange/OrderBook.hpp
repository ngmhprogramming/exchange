#pragma once
#include <exchange/common/Order.hpp>
#include <exchange/common/TradeReport.hpp>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace exchange {

class OrderBook {
  public:
    void matchOrder(Order &current_order, auto &other_side, auto can_match,
                    std::vector<TradeReport> &res);

    void addOrder(const Order &order, std::vector<TradeReport> &res);

    bool cancelOrder(uint64_t order_id);

    friend std::ostream &operator<<(std::ostream &os, const OrderBook &book);

  private:
    // Map Price -> Orders
    // Bids are in descending order
    // Asks are in ascending order
    std::map<uint64_t, std::list<Order>, std::greater<double>> bids;
    std::map<uint64_t, std::list<Order>> asks;

    // Order ID -> Order Pointer Mapping
    std::unordered_map<uint64_t, std::list<Order>::iterator> order_map;
};

} // namespace exchange