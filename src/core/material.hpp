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
        floating refraction_index;
        floating refraction_amount;

        bool reflection;
        floating reflection_amount;

        Material();
    };
}
