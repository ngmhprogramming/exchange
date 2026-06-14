#include <exchange.hpp>
#include <exchange/BinaryInputFeed.hpp>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    std::string data_file = "market_data.bin";
    if (argc > 1) {
        data_file = argv[1];
    }

    std::cout << "[Engine] Initializing order book\n";
    exchange::OrderBook ladder_book;
    exchange::BinaryInputFeed feed(ladder_book);
    std::vector<exchange::TradeReport> execution_reports;

    std::cout << "[Engine] Streaming transactions from " << data_file << "\n";

    // Attempt to stream and process the file target
    if (!feed.processFile(data_file, execution_reports)) {
        std::cerr << "[CRITICAL ERROR] Failed to open or parse binary file: "
                  << data_file << "\n";
        return 1;
    }

    // Operational statistics
    std::cout << "[Engine] Processing complete\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Total Trade Match Events Triggered: "
              << execution_reports.size() << "\n";
    std::cout << "--------------------------------------------------\n\n";

    // Dump final order book state
    std::cout << "Final Order Book Snapshot:\n";
    std::cout << ladder_book << "\n";

    return 0;
}