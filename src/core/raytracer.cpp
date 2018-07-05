#include "raytracer.hpp"

using namespace rt;

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

// local light model
fColor Raytracer::localLight(unsigned int polyIndex, Ray& viewRay, vec3& intersection, floating& u, floating& v, floating& w, vec3& normal)
{
    auto& polygons = this->scene->polygons;
    auto& light = this->scene->light;
    auto& ambient = this->scene->ambient;

    Polygon poly = polygons[polyIndex];

    vec3 viewVector = -1.0 * viewRay.direction;
    normal = glm::normalize(poly.n1 * u + poly.n2 * v + poly.n3 * w);

    floating rev = glm::degrees(glm::acos(glm::dot(viewVector, normal)));
    if (rev >= 180.0)
    {
        normal *= -1.0;
    }

    vec3 lightVector = glm::normalize(light.position - intersection);
    vec3 reflectVector = glm::normalize(2 * glm::dot(lightVector, normal) * normal - lightVector);

    vec3 a, d, s;
    a = poly.material->ambient * (ambient.intensity * ambient.color);

    floating theta = glm::dot(lightVector, normal);
    if (theta >= 0.0 && lightVisible(Ray(intersection + (0.1 * lightVector), lightVector)))
    {
        d = poly.material->diffuse * theta * (light.intensity * light.color);

        floating alpha = glm::dot(reflectVector, viewVector);
        if (alpha >= 0.0)
        {
            s = poly.material->specular * glm::pow(alpha, poly.material->shininess) * (light.intensity * light.color);
            floating x = glm::dot(reflectVector, viewVector);
            floating y = glm::pow(x, poly.material->shininess);

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

vec3 refract(vec3 view, vec3 normal, floating ior)
{
    floating cosi = glm::clamp(glm::dot(view, normal), -1.0, 1.0);
    floating etai = 1, etat = ior;
    vec3 n = normal;
    
    if (cosi < 0)
    {
        cosi = -cosi;
    }
    else
    {
        std::swap(etai, etat);
        n = -normal; 
    }
    
    floating eta = etai / etat;
    floating k = 1 - eta * eta * (1 - cosi * cosi);

    return k < 0.0 ? vec3(0.0) : eta * view + (eta * cosi - glm::sqrt(k)) * n;
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

    for (unsigned int p = 0; p < polygons.size(); p++)
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

    if (closestIndex != -1)
    {
        vec3 normal;
        fColor localColor;
        vec3 intersection = ray.origin + ray.direction * closestDistance;
        vec3 viewVector = ray.direction * -1.0;
        localColor = localLight(closestIndex, ray, intersection, cu, cv, cw, normal);

        fColor reflectColor(0.0);
        floating adaptiveReflection = adpT * polygons[closestIndex].material->reflection_amount;
        if (adaptiveReflection > this->adaptiveDepth)
        {
            vec3 reflectVector = glm::normalize(viewVector - 2 * glm::dot(viewVector, normal) * normal);
            trace(rt::Ray(intersection, reflectVector), reflectColor, depth + 1, adaptiveReflection);
        }
        
        fColor refractColor(0.0);
        floating adaptiveRefraction = adpT * polygons[closestIndex].material->refraction_amount;
        if (adaptiveRefraction > this->adaptiveDepth)
        {
            vec3 refractVector = refract(viewVector, normal, polygons[closestIndex].material->refraction_index);
            trace(rt::Ray(intersection, refractVector), refractColor, depth + 1, adaptiveRefraction);
        }

        out_color = glm::clamp(localColor + (polygons[closestIndex].material->reflection_amount * reflectColor) + (polygons[closestIndex].material->refraction_amount * refractColor), 0.0, 1.0);

        return true;
    }
    else
    {
        return false;
    }
}



/*
bool intersectSphere(const ray& r, const sphere& sph)
{
    if constexpr (CheckNormalized) {
        if (!glm::isNormalized(r.direction, 0.001f)) {
            throw std::logic_error("ray is not normalized");
        }
    }

    auto relative_sphere_pos = sph.position - r.origin;
    auto sphere_ray_dot = glm::dot(relative_sphere_pos, r.direction);

    // sphere is behind the ray
    if (sphere_ray_dot < 0.0f) {
        return false;
    }

    // distance to sphere center squared
    float d2 = glm::dot(relative_sphere_pos, relative_sphere_pos) -
        sphere_ray_dot * sphere_ray_dot;
    if (d2 >(sph.radius * sph.radius)) {
        return false;
    }

    float thc = sqrt((sph.radius * sph.radius) - d2);

    float t0 = sphere_ray_dot - thc;
    float t1 = sphere_ray_dot - thc;

    if (t0 > t1) {
        std::swap(t0, t1);
    }

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) {
            return false;
        }
    }

    return true;
}*/