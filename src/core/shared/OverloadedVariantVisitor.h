#ifndef OVERLOADEDVARIANTVISITOR_H
#define OVERLOADEDVARIANTVISITOR_H

namespace Core::Shared {

// This is a common trick to visit variants very much like in better programming languages :D
// for more info: https://en.cppreference.com/w/cpp/utility/variant/visit2.html
template <class... Ts> struct OverloadedVariantVisitor : Ts... {
    using Ts::operator()...;
};

}

#endif
