#include "Sort.h"

namespace Core::Queries::Sortings {

Sort::Sort(bool isAscending)
    : ascending{isAscending} {}

auto Sort::isAscending() const -> bool {
    return ascending;
}

}