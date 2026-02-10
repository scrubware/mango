

module;

#include <cstddef>

export module Mango.Core;

namespace Mango {
    export template <std::size_t N> struct FixedString {
        char value[N];
        constexpr FixedString(char const (&str)[N]) {
            for (std::size_t i = 0; i < N; ++i) value[i] = str[i];
        }
    };
}

