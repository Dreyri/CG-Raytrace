#pragma once

#include "defs.hpp"

namespace rt {
    struct Ray {
        vec3 origin;
        vec3 direction;

        Ray(const vec3& origin, const vec3& direction);
    };
} // namespace rt