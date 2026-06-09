#pragma once
#include <deque>
#include <exchange/common/Order.hpp>
#include <iostream>
#include <map>

namespace exchange {

class OrderBook {
  public:
    void addOrder(const Order &order);

  private:
    // Map Price -> Orders
    // Bids are in descending order
    // Asks are in ascending order
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    std::map<double, std::deque<Order>> asks;
};

} // namespace exchange