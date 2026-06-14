#pragma once
#include <cstdint>

namespace exchange {

enum class ProtocolOrderType : uint8_t { NewOrder = 1, Cancel = 2 };

enum class ProtocolSide : uint8_t { Buy = 1, Sell = 2 };

struct OrderPacket {
    uint64_t id;            // 8 bytes
    uint64_t timestamp;     // 8 bytes
    uint64_t price;         // 8 bytes
    uint32_t quantity;      // 4 bytes
    ProtocolSide side;      // 1 byte
    ProtocolOrderType type; // 1 byte
};

} // namespace exchange