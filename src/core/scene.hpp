#pragma once

#include <vector>

#include "light.hpp"
#include "camera.hpp"
#include "object.hpp"

namespace rt {
    class Scene {
    private:
        vec3 getBoundingCenter(Object& obj);
        floating getBoundingRadius(Object& obj);
        void setBoundingSphere(Object& obj);
    public:
        Camera camera;
        Light light;
        AmbientLight ambient;
        
        std::vector<Object> objects;
        std::vector<Polygon> polygons;

        void transform();
    };
} // namespace rt
