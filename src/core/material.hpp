#pragma once

#include "defs.hpp"

namespace rt
{
    struct Material
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        floating shininess;

        bool transparent;
        floating refraction_i;
        floating refraction_amount;

        floating reflection_amount;

        bool smoothed;
    };
}
