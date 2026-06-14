#include <exchange/OrderBook.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace exchange;

// 2.1 Cancel Single Resting Order
TEST(OrderBookTests, CancelSingleRestingOrder) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buy1{1, 1001, 1000000, 50, Side::Buy};
    book.addOrder(buy1, res);

    EXPECT_TRUE(book.cancelOrder(1));

    // Ensure order is completely dead
    // No trade should occur
    Order ask1{2, 1002, 1000000, 50, Side::Sell};
    book.addOrder(ask1, res);
    EXPECT_TRUE(res.empty());
}

// 2.2 Cancel Middle of Price Level Queue
TEST(OrderBookTests, CancelMiddleOfPriceLevelQueue) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buyA{1, 1003, 1000000, 10, Side::Buy};
    Order buyB{2, 1004, 1000000, 20, Side::Buy}; // Target inside list node
    Order buyC{3, 1005, 1000000, 30, Side::Buy};

    book.addOrder(buyA, res);
    book.addOrder(buyB, res);
    book.addOrder(buyC, res);

    EXPECT_TRUE(book.cancelOrder(2));

    // Send ask with quantity 25, should fill A and C without B
    Order ask1{4, 1006, 1000000, 25, Side::Sell};
    book.addOrder(ask1, res);

    ASSERT_EQ(res.size(), 2);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].quantity, 10);

    EXPECT_EQ(res[1].maker_id, 3);
    EXPECT_EQ(res[1].quantity, 15);
}

// 2.3 Cancel Queue Boundaries (Head and Tail)
TEST(OrderBookTests, CancelQueueBoundaries) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buyA{1, 1007, 1000000, 10, Side::Buy};
    Order buyB{2, 1008, 1000000, 20, Side::Buy};

    book.addOrder(buyA, res);
    book.addOrder(buyB, res);

    EXPECT_TRUE(book.cancelOrder(1)); // Cancel Head
    EXPECT_TRUE(book.cancelOrder(2)); // Cancel Tail

    // No trades should occur
    Order ask1{3, 1009, 1000000, 10, Side::Sell};
    book.addOrder(ask1, res);
    EXPECT_TRUE(res.empty());
}

// 2.4 Cancel Non-Existent ID
TEST(OrderBookTests, CancelNonExistentIDReturnsFalse) {
    OrderBook book;
    EXPECT_FALSE(book.cancelOrder(999999));
}

// 2.5 Cancel Post-Full Fill
TEST(OrderBookTests, CancelAlreadyFilledOrderReturnsFalse) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order ask1{1, 1010, 1000000, 10, Side::Sell};
    Order buy1{2, 1011, 1000000, 10, Side::Buy};

    book.addOrder(ask1, res);
    book.addOrder(buy1, res);

    // Trade occurred, order no longer exists
    EXPECT_FALSE(book.cancelOrder(1));
}