#pragma once
#include <cstdint>
#include <exchange/OrderBook.hpp>
#include <exchange/common/Protocol.hpp>
#include <vector>

namespace exchange {

class BinaryInputFeed {
  public:
    explicit BinaryInputFeed(OrderBook &book);

    // Interpret raw bytes as OrderPacket
    void processPacket(const uint8_t *raw_buffer,
                       std::vector<TradeReport> &reports);

    // Process file input
    bool processFile(const std::string &file_path,
                     std::vector<TradeReport> &reports);

  private:
    OrderBook &_book;
};

} // namespace exchange