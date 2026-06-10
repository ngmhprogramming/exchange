#include <exchange/OrderBook.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(OrderBookTests, ExactPriceMatch) {
    exchange::OrderBook book;
    std::vector<exchange::TradeReport> res;

    exchange::Order ask1{1, 1001, 150.00, 100, exchange::Side::Sell};
    exchange::Order buy1{2, 1002, 150.00, 100, exchange::Side::Buy};

    book.addOrder(ask1, res);
    EXPECT_TRUE(res.empty()); // No trade should occur

    book.addOrder(buy1, res); // Trade should be executed
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].quantity, 100);
    EXPECT_DOUBLE_EQ(res[0].price, 150.00);
    EXPECT_EQ(res[0].maker_id, 1);
    EXPECT_EQ(res[0].taker_id, 2);
}

TEST(OrderBookTests, PartialFillLeavesRemainder) {
    exchange::OrderBook book;
    std::vector<exchange::TradeReport> res;

    exchange::Order ask2{3, 1003, 200.00, 100, exchange::Side::Sell};
    exchange::Order buy2{4, 1004, 200.00, 40, exchange::Side::Buy};

    book.addOrder(ask2, res);
    book.addOrder(buy2, res);

    // Trade should partially fill existing order
    ASSERT_EQ(res.size(), 1);
    EXPECT_EQ(res[0].quantity, 40);
    EXPECT_DOUBLE_EQ(res[0].price, 200.00);
    EXPECT_EQ(res[0].maker_id, 3);
    EXPECT_EQ(res[0].taker_id, 4);
}

TEST(OrderBookTests, NonCrossingOrdersRestPassively) {
    exchange::OrderBook book;
    std::vector<exchange::TradeReport> res;

    exchange::Order buy3{5, 1005, 190.00, 50, exchange::Side::Buy};
    exchange::Order sell3{5, 1005, 200.00, 50, exchange::Side::Buy};

    book.addOrder(buy3, res);
    book.addOrder(sell3, res);
    EXPECT_TRUE(res.empty());
}

TEST(OrderBookTests, MassLiquidityGeneration) {
    exchange::OrderBook ladder_book;
    std::vector<exchange::TradeReport> res;

    for (int i = 0; i < 50; i++) {
        double price = i;
        uint32_t qty = i * 100;
        exchange::Side side =
            (i <= 25) ? exchange::Side::Buy : exchange::Side::Sell;

        exchange::Order o{static_cast<uint64_t>(i),
                          static_cast<uint64_t>(1000 + i), price, qty, side};
        ladder_book.addOrder(o, res);

        EXPECT_TRUE(res.empty());
    }
}

TEST(SanitizerVerification, TriggerASan) {
    int volatile *array = new int[10];
    array[0] = 42;

    // Intentionally reading out-of-bounds (index 15 on an array of size 10)
    int volatile leak = array[15];
    (void)leak;

    delete[] array;
}