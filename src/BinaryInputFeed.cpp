#include <exchange/BinaryInputFeed.hpp>
#include <fstream>

namespace exchange {

BinaryInputFeed::BinaryInputFeed(OrderBook &book) : _book(book) {}

void BinaryInputFeed::processPacket(const uint8_t *raw_buffer,
                                    std::vector<TradeReport> &reports) {
    const auto *packet = reinterpret_cast<const OrderPacket *>(raw_buffer);
    if (packet->type == ProtocolOrderType::NewOrder) {
        Side core_side =
            (packet->side == ProtocolSide::Buy) ? Side::Buy : Side::Sell;
        Order order{packet->id, packet->timestamp, packet->price,
                    packet->quantity, core_side};
        _book.addOrder(order, reports);
    } else if (packet->type == ProtocolOrderType::Cancel) {
        _book.cancelOrder(packet->id);
    }
}

bool BinaryInputFeed::processFile(const std::string &file_path,
                                  std::vector<TradeReport> &reports) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Allocate stack buffer for a packet
    constexpr size_t packet_size = sizeof(OrderPacket);
    uint8_t buffer[packet_size];

    // Read file input
    while (file.read(reinterpret_cast<char *>(buffer), packet_size)) {
        processPacket(buffer, reports);
    }
    return true;
}

} // namespace exchange