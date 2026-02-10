//
// Created by jarboola on 2/4/26.
//

module;

#include <raylib.h>

export module Mango.Input;

import Mango.Core;

namespace Mango {

    enum class Key {

    };

    export template <FixedString key> bool Released() {

    }

    export template <FixedString key> bool Pressed() {

    }

    export template <FixedString key> bool Held() {
        //return IsKeyDown(key);
    }

}


void test() {
    if (Mango::Held<"fire">()) {

    }

}