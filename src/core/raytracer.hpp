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
#include "sphere.hpp"

namespace rt
{
    class RenderTarget
    {
    public:
        virtual unsigned int getWidth() = 0;
        virtual unsigned int getHeight() = 0;
        virtual void setPixel(unsigned int w, unsigned int h, floating r, floating g, floating b) = 0;
    };

    class Raytracer
    {
    private:

    public:
        std::shared_ptr<Scene> scene;

        unsigned int maxDepth;
        double adaptiveDepth;
        bool smoothing;
        bool reflection;
        bool refraction;

        Raytracer();

        bool intersectTriangle(Ray& ray, Polygon& poly, floating& t, floating& u, floating& v, floating& w);       
        bool lightVisible(Ray& lightRay);
        Material& getSurfaceTriangle(const unsigned int polyIndex, const Ray& viewRay, const vec3& intersection, const floating u, const floating v, const floating w, vec3& normal);
        fColor localLight(Ray& viewRay, const vec3& intersection, const vec3& normal, Material& material);
        bool trace(Ray& ray, fColor& out_color, unsigned int depth, floating adpT);
        bool intersectSphere(const Ray& r, const Sphere& sph);

        virtual void render(std::shared_ptr<rt::RenderTarget> target) = 0;
    };
}