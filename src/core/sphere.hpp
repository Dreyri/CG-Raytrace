#pragma once

#include "defs.hpp"

namespace rt {
    struct Sphere {
        vec3 position;
        floating radius;

        Sphere() = default;
        Sphere(vec3& position, floating radius);
    };
} // namespace rt
