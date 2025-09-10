#ifndef MEDIUMCARDSVIEWMODELFACTORY_H
#define MEDIUMCARDSVIEWMODELFACTORY_H

#include "media-list/LibraryMediumCard.h"
#include "model/Medium.h"

using Core::Model::Medium;

namespace Gui::Components {

class MediumCardsViewModelFactory {
  public:
    [[nodiscard]] static auto createMediumCards(const std::vector<const Medium *> &media)
        -> std::vector<LibraryMediumCard::MediumCardViewModel>;
};

}

#endif