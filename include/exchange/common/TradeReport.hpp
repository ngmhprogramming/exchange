#pragma once
#include <cstdint>

namespace exchange {

struct TradeReport {
    uint64_t maker_id;
    uint64_t taker_id;
    uint64_t timestamp;
    double price;
    uint32_t quantity;
};

} // namespace exchange