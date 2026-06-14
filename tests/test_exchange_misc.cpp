#include <exchange/OrderBook.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <vector>

using namespace exchange;

// 3.1 Ladder View Output
TEST(OrderBookTests, LadderViewStringSnapshotValidation) {
    OrderBook book;
    std::vector<TradeReport> res;

    Order ask1{1, 1001, 1500000, 50, Side::Sell}; // 150.00
    Order buy1{2, 1002, 1490000, 30, Side::Buy};  // 149.00

    book.addOrder(ask1, res);
    book.addOrder(buy1, res);

    // Intercept standard output stream
    std::stringstream ss;
    ss << book;
    std::string output = ss.str();

    // Verify outputs exist
    EXPECT_NE(output.find("Bid"), std::string::npos);
    EXPECT_NE(output.find("Ask"), std::string::npos);
    EXPECT_NE(output.find("150.00"), std::string::npos);
    EXPECT_NE(output.find("149.00"), std::string::npos);
    EXPECT_NE(output.find("50"), std::string::npos);
    EXPECT_NE(output.find("30"), std::string::npos);
}