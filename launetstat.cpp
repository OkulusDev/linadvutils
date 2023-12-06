#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <iterator>

void printColored(const std::string& text, const std::string& color) {
    std::cout << "\033[" << color << "m" << text << "\033[0m";
}

void printConnections(const std::vector<std::string>& lines) {
    std::cout << std::setw(10) << "Protocol"
              << std::setw(25) << "Local Address"
              << std::setw(15) << "Local Port"
              << std::setw(25) << "Foreign Address"
              << std::setw(15) << "Foreign Port"
              << std::setw(15) << "Status" << std::endl;

    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());

        if (tokens.size() >= 4) {
            std::cout << std::setw(10) << tokens[0]
                      << std::setw(25) << tokens[1]
                      << std::setw(15) << tokens[2]
                      << std::setw(25) << tokens[3]
                      << std::setw(15) << tokens[4];

            if (tokens[5] == "LISTEN") {
                printColored(std::setw(15) + tokens[5], "1;32");
            } else if (tokens[5] == "ESTABLISHED") {
                printColored(std::setw(15) + tokens[5], "1;36");
            } else {
                printColored(std::setw(15) + tokens[5], "1;31");
            }

            std::cout << std::endl;
        }
    }
}

void printNetworkStats() {
    std::ifstream file("/proc/net/tcp");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    printConnections(lines);
}

int main() {
    printNetworkStats();
    return 0;
}
