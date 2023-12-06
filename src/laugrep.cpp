/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: Grep analogue
 *  File: src/laugrep.cpp
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple grep alternative for finding words from strings */

#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <iterator>
#include <iomanip>

void printColored(const std::string& line, const std::string& pattern) {
    std::regex re(pattern);
    std::sregex_iterator begin(line.begin(), line.end(), re);
    std::sregex_iterator end;
    
    std::string::const_iterator lineIt = line.begin();
    
    while (begin != end) {
        std::sregex_iterator match = begin++;
        std::cout << std::string(lineIt, match->prefix().first);
        std::cout << "\033[1;31m" << match->str() << "\033[0m";
        lineIt = match->suffix().first;
    }
    
    std::cout << std::string(lineIt, line.end()) << std::endl;
}

int main(int argc, char** argv) {
    std::string pattern;
    std::string line;

    if (argc < 2) {
        while (std::getline(std::cin, line)) {
            printColored(line, pattern);
        }
    } else {
        pattern = argv[1];
        
        if (argc > 2) {
            line = argv[2];
            
            printColored(line, pattern);
        } else {
            while (std::getline(std::cin, line)) {
                printColored(line, pattern);
            }
        }
    }

    return 0;
}
