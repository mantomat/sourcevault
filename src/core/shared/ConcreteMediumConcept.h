#ifndef CONCRETEMEDIUMCONCEPT_H
#define CONCRETEMEDIUMCONCEPT_H

#include "model/Medium.h"

#include <concepts>

using Core::Model::Medium;

namespace Core::Shared {

template <typename T>
concept ConcreteMedium = std::derived_from<T, Medium> && !std::is_same_v<T, Medium>;

}

#endif