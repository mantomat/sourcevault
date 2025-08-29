#include "FieldScoreCalculatorRepl.h"

#include <iostream>
#include <string>

using Core::Queries::Search::ScoreCalculatorConfigs;
using Tools::FieldScoreCalculatorRepl;

auto main() -> int {
    std::cout << "SourceVault Field Score Calculator REPL Tester" << '\n';
    std::cout << "Enter search term and field, separated by a semicolon `;`." << '\n';
    std::cout << "Type 'quit' to exit." << '\n';

    ScoreCalculatorConfigs configs{};

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line) && line != "quit") {
        std::string searchTermStr;
        std::string fieldStr;

        size_t spacePos{line.find(';')};
        if (spacePos != std::string::npos) {
            searchTermStr = line.substr(0, spacePos);
            fieldStr = line.substr(spacePos + 1);
        } else {
            std::cout << "Invalid input. Please provide a search term and a field." << '\n';
            continue;
        }

        QString searchTerm{QString::fromStdString(searchTermStr)};
        QString field{QString::fromStdString(fieldStr)};

        FieldScoreCalculatorRepl tester{std::move(searchTerm), configs};

        tester.runAndPrintDetails(field);
    }

    return 0;
}