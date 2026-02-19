//
// Created by jarboola on 2/4/26.
//

module;

#include <utility>

export module Mango.Math;

namespace Mango {

    export struct Vec2 {
        float x;
        float y;

        Vec2(): x(0), y(0) {}
        Vec2(const float x, const float y) : x(x), y(y) {}
        explicit Vec2(const float v): x(v), y(v) {}
    };

    export struct Vec3 {
        float x;
        float y;
        float z;

        Vec3(): x(0), y(0), z(0) {}
        Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
        explicit Vec3(const float v): x(v), y(v), z(v) {}
    };

    export template <int D = 2, typename... Ts> auto Vec(Ts&&... values) {
        if constexpr (D == 2) {
            return Vec2{std::forward<Ts>(values)...};
        }
        if constexpr (D == 3) {
            return Vec3{std::forward<Ts>(values)...};
        }
    }
}