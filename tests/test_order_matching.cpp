#include <exchange/OrderBook.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace exchange;

// 1.1 Add Single Order
TEST(OrderBookTests, AddSingleOrderRestsPassively) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buy1{1, 1001, 1500000, 100, Side::Buy}; // 150.00
    book.addOrder(buy1, res);

    EXPECT_TRUE(res.empty());
}

// 1.2 Exact Match (Bid Then Ask)
TEST(OrderBookTests, ExactMatchBidThenAsk) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buy1{1, 1001, 1000000, 50, Side::Buy};  // 100.00
    Order ask1{2, 1002, 1000000, 50, Side::Sell}; // 100.00

    book.addOrder(buy1, res);
    EXPECT_TRUE(res.empty());

    book.addOrder(ask1, res);
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].taker_id, 2);
    EXPECT_EQ(res[0].price, 1000000);
    EXPECT_EQ(res[0].quantity, 50);
}

// 1.3 Exact Match (Ask Then Bid)
TEST(OrderBookTests, ExactMatchAskThenBid) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order ask1{1, 1001, 1000000, 50, Side::Sell}; // 100.00
    Order buy1{2, 1002, 1000000, 50, Side::Buy};  // 100.00

    book.addOrder(ask1, res);
    book.addOrder(buy1, res);

    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].taker_id, 2);
    EXPECT_EQ(res[0].price, 1000000);
    EXPECT_EQ(res[0].quantity, 50);
}

// 1.4 Partial Fill (Aggressive Bid)
TEST(OrderBookTests, PartialFillAggressiveBid) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order ask1{1, 1003, 2000000, 100, Side::Sell}; // 200.00
    Order buy1{2, 1004, 2000000, 40, Side::Buy};   // 200.00

    book.addOrder(ask1, res);
    book.addOrder(buy1, res);

    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].taker_id, 2);
    EXPECT_EQ(res[0].quantity, 40);
    EXPECT_EQ(res[0].price, 2000000);
}

// 1.5 Partial Fill (Aggressive Ask)
TEST(OrderBookTests, PartialFillAggressiveAsk) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buy1{1, 1003, 2000000, 100, Side::Buy}; // 200.00
    Order ask1{2, 1004, 2000000, 40, Side::Sell}; // 200.00

    book.addOrder(buy1, res);
    book.addOrder(ask1, res);

    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].taker_id, 2);
    EXPECT_EQ(res[0].quantity, 40);
    EXPECT_EQ(res[0].price, 2000000);
}

// 1.6 Non-Intersecting Orders
TEST(OrderBookTests, NonIntersectingOrdersPreserveSpread) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buy1{1, 1005, 1900000, 50, Side::Buy};  // 190.00
    Order ask1{2, 1006, 2000000, 50, Side::Sell}; // 200.00

    book.addOrder(buy1, res);
    EXPECT_TRUE(res.empty());

    book.addOrder(ask1, res);
    EXPECT_TRUE(res.empty());
}

// 1.7 Price Improvement
TEST(OrderBookTests, PriceImprovementTakerMatchesAtMakerPrice) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order ask1{1, 1007, 1000000, 50, Side::Sell}; // Rest at 100.00
    Order buy1{2, 1008, 1050000, 50, Side::Buy};  // Aggressive cap 105.00

    book.addOrder(ask1, res);
    book.addOrder(buy1, res);

    // Should execute at maker price
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].price, 1000000);
}

// 1.8 Walking the Book
TEST(OrderBookTests, WalkTheBookSweepMultipleLevels) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order askA{1, 1009, 1000000, 10, Side::Sell}; // 100.00
    Order askB{2, 1010, 1010000, 10, Side::Sell}; // 101.00
    Order buy1{3, 1011, 1050000, 15, Side::Buy};  // Sweeper 105.00

    book.addOrder(askA, res);
    book.addOrder(askB, res);
    book.addOrder(buy1, res);

    ASSERT_EQ(res.size(), 2);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].price, 1000000);
    EXPECT_EQ(res[0].quantity, 10);

    EXPECT_EQ(res[1].maker_id, 2);
    EXPECT_EQ(res[1].price, 1010000);
    EXPECT_EQ(res[1].quantity, 5);
}

// 1.9 Time Priority (FIFO)
TEST(OrderBookTests, TimePriorityFIFOMatchingAtSamePrice) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order buyA{1, 1012, 500000, 100, Side::Buy}; // Early Bid $50.00
    Order buyB{2, 1013, 500000, 100, Side::Buy}; // Later Bid $50.00
    Order ask1{3, 1014, 500000, 50, Side::Sell};

    book.addOrder(buyA, res);
    book.addOrder(buyB, res);
    book.addOrder(ask1, res);

    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].maker_id, 1); // Fill order A instead of B
    EXPECT_EQ(res[0].quantity, 50);
}