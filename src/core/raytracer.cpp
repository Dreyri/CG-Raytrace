#include "raytracer.hpp"

using namespace rt;

rt::Raytracer::Raytracer() : maxDepth{8}, adaptiveDepth{0.05}
{
}

bool Raytracer::intersectTriangle(Ray& ray, Polygon& poly, floating& t, floating& u, floating& v, floating& w)
{
    const floating EPSILON = 0.0000001;

    vec3 edge1 = poly.v2 - poly.v1;
    vec3 edge2 = poly.v3 - poly.v1;

    vec3 h = glm::cross(ray.direction, edge2);
    floating det = glm::dot(edge1, h);
    if (glm::abs(det) < EPSILON) // Ray parallel to triangle
    {
        return false;
    }

    floating f = 1 / det;
    vec3 s = ray.origin - poly.v1;
    u = f * (glm::dot(s, h));
    if (u < 0.0 || u > 1.0) // Point outside triangle area
    {
        return false;
    }

    vec3 q = glm::cross(s, edge1);
    v = f * glm::dot(ray.direction, q);
    if (v < 0.0 || u + v > 1.0)  // Point outside triangle area
    {
        return false;
    }

    // At this stage we can compute distance t to intersection point.
    t = f * glm::dot(edge2, q);
    w = 1.0 - u - v;
    if (t > EPSILON) // ray intersection
    {
        //IntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
    {
        return false;
    }
}

// true if no obstruction along lightRay e.g. not in shadow
bool Raytracer::lightVisible(Ray& lightRay)
{
    auto& polygons = this->scene->polygons;
    auto& light = this->scene->light;

    floating t, u, v, w;

    for (auto poly : polygons)
    {
        if (intersectTriangle(lightRay, poly, t, u, v, w))
        {
            return false;
        }
    }
    return true;
}

Material& Raytracer::getSurfaceTriangle(const unsigned int polyIndex, const Ray & viewRay, const vec3 & intersection, const floating u, const floating v, const floating w, vec3 & normal)
{
    if (smoothing)
    {
        normal = glm::normalize((this->scene->polygons[polyIndex].n1 * w) + (this->scene->polygons[polyIndex].n2 * u) + (this->scene->polygons[polyIndex].n3 * v));
    }
    else
    {
        Polygon& poly = this->scene->polygons[polyIndex];
        normal = glm::normalize(glm::cross(poly.v1 - poly.v2, poly.v1 - poly.v3));
    }    
    return *this->scene->polygons[polyIndex].material;
}

// local light model
fColor Raytracer::localLight(Ray& viewRay, const vec3& intersection, const vec3& normal, Material& material)
{
    auto& light = this->scene->light;
    auto& ambient = this->scene->ambient;    

    vec3& viewVector = viewRay.direction;
    vec3 lightVector = glm::normalize(light.position - intersection);
    vec3 reflectVector = glm::reflect(lightVector, normal);

    vec3 a, d, s;
    a = material.ambient * (ambient.intensity * ambient.color);

    floating theta = glm::dot(lightVector, normal);
    if (theta >= 0.0 && lightVisible(Ray(intersection + (0.01 * lightVector), lightVector)))
    {
        d = material.diffuse * theta * (light.intensity * light.color);

        floating alpha = glm::dot(reflectVector, viewVector);
        if (alpha >= 0.0)
        {
            s = material.specular * glm::pow(alpha, material.shininess) * (light.intensity * light.color);
            floating x = glm::dot(reflectVector, viewVector);
            floating y = glm::pow(x, material.shininess);

            return glm::clamp(a + d + s, 0.0, 1.0);
        }
        else
        {
            return glm::clamp(a + d, 0.0, 1.0);
        }
    }
    else
    {
        return glm::clamp(a, 0.0, 1.0);
    }
}

// returns true if it hits an object, color
bool Raytracer::trace(Ray& ray, fColor& out_color, unsigned int depth, floating adpT)
{
    if (depth > this->maxDepth)
    {
        return false;
    }

    auto& polygons = this->scene->polygons;
    auto& light = this->scene->light;
    auto& ambient = this->scene->ambient;

    floating closestDistance = 99999999.0;
    unsigned int closestIndex = -1;
    floating distance, u, v, w;
    floating cu, cv, cw;

    for (auto& obj : this->scene->objects)
    {
        if(obj.mesh.type == MeshType::noBounding || intersectSphere(ray, obj.bounding))
        { 
            for (unsigned int p = obj.poly_first; p <= obj.poly_last; p++)
            {
                if (intersectTriangle(ray, polygons[p], distance, u, v, w))
                {
                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestIndex = p;
                        cu = u;
                        cv = v;
                        cw = w;
                    }
                }
            }
        }
    }

    if (closestIndex != -1)
    {
        vec3 normal;
        vec3 intersection = ray.origin + ray.direction * closestDistance;
        vec3& viewVector = ray.direction;
        Material& material = getSurfaceTriangle(closestIndex, ray, intersection, cu, cv, cw, normal);

        fColor localColor = localLight(ray, intersection, normal, material);
        
        floating adaptiveReflection = adpT * material.reflection_amount;
        floating adaptiveRefraction = adpT * material.refraction_amount;
        fColor reflectColor(0.0);
        fColor refractColor(0.0);

        floating outsideDensity = 1.0;
        floating insideDensity = material.refraction_index;
        floating viewProjection = glm::clamp(glm::dot(viewVector, normal), -1.0, 1.0);
        vec3 outNormal = normal;
        if (viewProjection > 0.0)
        {
            outNormal = -normal;
            std::swap(outsideDensity, insideDensity);
        }

        // Reflection
        if (this->reflection && adaptiveReflection > this->adaptiveDepth)
        {
            vec3 reflectVector = glm::reflect(viewVector, outNormal);
            if (trace(rt::Ray(intersection, reflectVector), reflectColor, depth + 1, adaptiveReflection))
            {
                localColor += (material.reflection_amount * reflectColor);
            }
            /*else
            {
                localColor += (material.reflection_amount * this->scene->background);
            }*/
        }

        // Refraction
        if(polygons[closestIndex].material->transparent && this->refraction  && adaptiveRefraction > this->adaptiveDepth)
        {
            vec3 refractVector = glm::refract(viewVector, outNormal, glm::clamp(outsideDensity / insideDensity, -1.0, 1.0));
            if (trace(rt::Ray(intersection + (0.1*refractVector), refractVector), refractColor, depth + 1, adaptiveRefraction))
            {
                localColor += (material.refraction_amount * refractColor);
            }
            else
            {
                localColor += (material.refraction_amount * this->scene->background);
            }
        }
        
        out_color = glm::clamp(localColor, 0.0, 1.0);
        return true;        
    }
    else
    {
        out_color = this->scene->background;
        return false;
    }
}

bool Raytracer::intersectSphere(const Ray& r, const Sphere& sph)
{
    auto relative_sphere_pos = sph.position - r.origin;
    auto sphere_ray_dot = glm::dot(relative_sphere_pos, r.direction);

    // sphere is behind the ray
    if (sphere_ray_dot < 0.0f) {
        return false;
    }

    // distance to sphere center squared
    floating d2 = glm::dot(relative_sphere_pos, relative_sphere_pos) -
        sphere_ray_dot * sphere_ray_dot;
    if (d2 >(sph.radius * sph.radius)) {
        return false;
    }

    floating thc = sqrt((sph.radius * sph.radius) - d2);

    floating t0 = sphere_ray_dot - thc;
    floating t1 = sphere_ray_dot - thc;

    if (t0 > t1) {
        std::swap(t0, t1);
    }

    if (t0 < 0.0) {
        t0 = t1;
        if (t0 < 0.0) {
            return false;
        }
    }

    return true;
}