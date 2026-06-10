#pragma once
#include <deque>
#include <exchange/common/Order.hpp>
#include <exchange/common/TradeReport.hpp>
#include <iostream>
#include <map>
#include <vector>

namespace exchange {

class OrderBook {
  public:
    void addOrder(const Order &order, std::vector<TradeReport> &res);
    friend std::ostream &operator<<(std::ostream &os, const OrderBook &book);

  private:
    // Map Price -> Orders
    // Bids are in descending order
    // Asks are in ascending order
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    std::map<double, std::deque<Order>> asks;
};

} // namespace exchange