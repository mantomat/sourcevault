#include "MediumCardsViewModelFactory.h"

#include "MediumToCardVisitor.h"

#include <ranges>

namespace Gui::Components {

auto MediumCardsViewModelFactory::createMediumCards(const std::vector<const Medium *> &media)
    -> std::vector<LibraryMediumCard::MediumCardViewModel> {

    MediumToCardVisitor visitor;
    auto mediumCardsView{media | std::views::transform([&visitor](const auto &medium) {
                             medium->accept(visitor);
                             return visitor.getCardData();
                         })};
    return std::vector<LibraryMediumCard::MediumCardViewModel>{mediumCardsView.begin(),
                                                               mediumCardsView.end()};
}

}