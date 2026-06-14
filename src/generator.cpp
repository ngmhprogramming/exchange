#include <exchange.hpp>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    const std::string filename = "market_data.bin";
    std::ofstream out(filename, std::ios::binary);

    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing\n";
        return 1;
    }

    std::cout << "[Generator] Synthesizing 50 historical order packets\n";

    for (int i = 0; i < 50; i++) {
        uint64_t price = i * 10000;
        uint32_t qty = i * 100;
        exchange::ProtocolSide side = (i <= 25) ? exchange::ProtocolSide::Buy
                                                : exchange::ProtocolSide::Sell;
        exchange::OrderPacket packet;
        packet.id = static_cast<uint64_t>(i);
        packet.timestamp = static_cast<uint64_t>(1000 + i);
        packet.price = price;
        packet.quantity = qty;
        packet.side = side;
        packet.type = exchange::ProtocolOrderType::NewOrder;
        out.write(reinterpret_cast<const char *>(&packet),
                  sizeof(exchange::OrderPacket));
    }

    std::cout << "[Generator] Successfully exported data to " << filename
              << "\n";
    out.close();
    return 0;
}