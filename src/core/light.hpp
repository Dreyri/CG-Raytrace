#pragma once

#include "defs.hpp"

namespace rt
{
    struct Light
    {
        vec3 position;
        fColor color;
        floating intensity;
    };

    struct AmbientLight
    {
        fColor color;
        floating intensity;
    };
}

