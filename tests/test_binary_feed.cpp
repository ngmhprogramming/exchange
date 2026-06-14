#include <cstring>
#include <exchange/BinaryInputFeed.hpp>
#include <exchange/OrderBook.hpp>
#include <exchange/common/Protocol.hpp>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

using namespace exchange;

// 4.1 Process Crossing Stream
TEST(BinaryFeedTests, ProcessBinaryCrossingStream) {
    OrderBook book;
    BinaryInputFeed feed(book);
    std::vector<TradeReport> reports;

    // Setup memory buffer with raw packets
    uint8_t buffer[32];
    std::memset(buffer, 0, sizeof(buffer));
    // Insert bid
    auto *packet = reinterpret_cast<OrderPacket *>(buffer);
    packet->id = 5001;
    packet->timestamp = 1600000000;
    packet->price = 1050000; // 105.00
    packet->quantity = 75;
    packet->side = ProtocolSide::Buy;
    packet->type = ProtocolOrderType::NewOrder;
    feed.processPacket(buffer, reports);
    EXPECT_TRUE(reports.empty());

    // Insert perfectly crossing ask
    packet->id = 5002;
    packet->side = ProtocolSide::Sell;
    feed.processPacket(buffer, reports);

    // Trade should have executed
    ASSERT_EQ(reports.size(), 1);
    EXPECT_EQ(reports[0].maker_id, 5001);
    EXPECT_EQ(reports[0].taker_id, 5002);
    EXPECT_EQ(reports[0].price, 1050000);
    EXPECT_EQ(reports[0].quantity, 75);
}

// 4.2 Process Cancellation Stream
TEST(BinaryFeedTests, ProcessBinaryCancellationStream) {
    OrderBook book;
    BinaryInputFeed feed(book);
    std::vector<TradeReport> reports;

    uint8_t buffer[32];
    std::memset(buffer, 0, sizeof(buffer));
    auto *packet = reinterpret_cast<OrderPacket *>(buffer);

    // Add a bid
    packet->id = 7001;
    packet->price = 2000000; // 200.00
    packet->quantity = 10;
    packet->side = ProtocolSide::Buy;
    packet->type = ProtocolOrderType::NewOrder;
    feed.processPacket(buffer, reports);

    // Send cancellation request
    packet->id = 7001;
    packet->type = ProtocolOrderType::Cancel;
    feed.processPacket(buffer, reports);

    // Verify cancellation worked and no trade is executed
    packet->id = 7002;
    packet->side = ProtocolSide::Sell;
    packet->type = ProtocolOrderType::NewOrder;
    feed.processPacket(buffer, reports);

    EXPECT_TRUE(reports.empty());
}

// 4.3 Process File Input Crossing Stream
TEST(BinaryFeedTests, ProcessBinaryFileStream) {
    OrderBook book;
    BinaryInputFeed feed(book);
    std::vector<TradeReport> reports;

    const std::string mock_filename = "mock_market_data.bin";

    // Generate binary file with crossing input
    {
        std::ofstream out_file(mock_filename, std::ios::binary);
        ASSERT_TRUE(out_file.is_open());
        OrderPacket p1{8001,
                       100001,
                       500000,
                       20,
                       ProtocolSide::Buy,
                       ProtocolOrderType::NewOrder};
        OrderPacket p2{8002,
                       100002,
                       500000,
                       20,
                       ProtocolSide::Sell,
                       ProtocolOrderType::NewOrder};
        out_file.write(reinterpret_cast<const char *>(&p1),
                       sizeof(OrderPacket));
        out_file.write(reinterpret_cast<const char *>(&p2),
                       sizeof(OrderPacket));
    }

    // Process file
    bool success = feed.processFile(mock_filename, reports);
    EXPECT_TRUE(success);
    std::remove(mock_filename.c_str());

    // Check that trade occurred
    ASSERT_EQ(reports.size(), 1);
    EXPECT_EQ(reports[0].maker_id, 8001);
    EXPECT_EQ(reports[0].taker_id, 8002);
    EXPECT_EQ(reports[0].quantity, 20);
}