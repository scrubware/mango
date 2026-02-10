//
// Created by jarboola on 2/4/26.
//

module;

#include <functional>
#include <raylib.h>

export module Mango.Cam;

import Mango.Math;

namespace Mango {

    export struct Cam final {

    private:
        Camera2D camera{0,0};
        Vec2 origin;

    public:

        // Frames the camera in screen space coordinates.
        Cam& Frame(const float width, const float height) {
            camera.target = {-width/2,-height/2};
            return *this;
        }

        Cam& Track(const Vec2) {
            return *this;
        }

        Cam& Track(const std::vector<Vec2>& positions) {
            return *this;
        }

        Cam& MinSize(const Vec2 min_size) {
            return *this;
        }

        void Draw(const std::function<void()>& lambda) const {
            BeginMode2D(camera);
            lambda();
            EndMode2D();
        }
    };

}

