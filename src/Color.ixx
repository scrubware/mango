//
// Created by jarboola on 2/4/26.
//

module;

#include <functional>
#include <raylib.h>

export module Mango.Color;

namespace Mango {

    export struct Col final {
        Color color;
        constexpr explicit Col(const Color color): color(color) {}
    };

    export constexpr Col White{WHITE};
    export constexpr Col Black{BLACK};
}

