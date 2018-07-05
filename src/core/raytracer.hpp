#pragma once

#include <vector>
#include <memory>

#include "defs.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "scene.hpp"

namespace rt
{
    class RenderTarget
    {
    public:
        virtual unsigned int getWidth() = 0;
        virtual unsigned int getHeight() = 0;
        virtual void setPixel(unsigned int w, unsigned int h, float r, float g, float b) = 0;
    };

    class Raytracer
    {
    private:

    public:
        std::shared_ptr<Scene> scene;

        bool intersectTriangle(Ray& ray, Polygon& poly, floating& t, floating& u, floating& v, floating& w);       
        bool lightVisible(Ray& lightRay);
        fColor localLight(unsigned int polyIndex, Ray& viewRay, vec3& intersection);
        bool trace(Ray& ray, fColor& out_color);
        //bool intersectSphere(const ray& r, const sphere& sph);

        virtual void render(std::shared_ptr<rt::RenderTarget> target) = 0;
    };
}