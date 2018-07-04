#include "raytracer_simple.hpp"

#include <iostream>

struct Ray
{
    glm::dvec3 origin;
    glm::dvec3 direction;

    Ray(glm::dvec3 origin, glm::dvec3 direction)
    {
        this->origin = origin;
        this->direction = direction;
    }
};

struct Light
{
    glm::dvec3 position;
    glm::dvec3 color;
    double intensity;
};

struct AmbientLight
{
    glm::dvec3 color;
    double intensity;
};

struct Material
{
    glm::dvec3 ambient;
    glm::dvec3 diffuse;
    glm::dvec3 specular;
    double shininess;

    bool smoothed;
};

struct Polygon
{
    glm::dvec3 v1, v2, v3;
    glm::dvec3 normal;

    std::shared_ptr<Material> material;
};

struct Camera
{
    glm::dvec3 position;
    glm::dvec3 direction;
    glm::dvec3 up{0.0, 1.0, 0.0};
    double height, width;
    unsigned int imageHeight, imageWidth;
    unsigned int fov;

    glm::dvec3 origin;
    glm::dvec3 topLeft;
    glm::dvec3 stepPixelRight;
    glm::dvec3 stepPixelDown;

    void calculateDerived()
    {
        direction = glm::normalize(direction); // normalize view direction just in case
        glm::dvec3 toLeft = glm::cross(up, direction); // vector that points to the left in image plane

        // origin point of eye behind image rectangle        
        double n = -1.0 * ( (width / 2) / tan((double)fov / 2) );
        origin = position + (n * direction);
        
        // top left corner of image rectangle
        topLeft = ((width / 2) * toLeft) + ((height / 2) * up) + position; // Half width to the left, half height up from position

        // vector to describe a one pixel step to the right
        stepPixelRight = (-1.0f * (width / (float)imageWidth)) * toLeft;

        // vector to describe a one pixel step downwards
        stepPixelDown = (-1.0f * (height / (float)imageHeight)) * up;
    }

    // 0,0 at top left, y points down, x to the right
    glm::dvec3 centerOfPixel(unsigned int y, unsigned int x)
    {
        return topLeft + ((double)x * stepPixelRight) + (0.5 * stepPixelRight) // x pixel to the right, starting with 0, add half a step to get to center of pixel
                       + ((double)y * stepPixelDown) + (0.5 * stepPixelDown);  // y pixel down, starting with 0, add half a step to get to center of pixel
    }
};

bool RayIntersectsTriangle(Ray& ray, Polygon& poly, double& t, double& u, double& v, double& w)
{
    const double EPSILON = 0.0000001;

    glm::dvec3 edge1 = poly.v2 - poly.v1;
    glm::dvec3 edge2 = poly.v3 - poly.v1;
    
    glm::dvec3 h = glm::cross(ray.direction, edge2);
    double det = glm::dot(edge1, h);
    if (glm::abs(det) < EPSILON) // Ray parallel to triangle
    {
        return false;
    }

    double f = 1 / det;
    glm::dvec3 s = ray.origin - poly.v1;
    u = f * (glm::dot(s, h));
    if (u < 0.0 || u > 1.0) // Point outside triangle area
    {
        return false;
    }

    glm::dvec3 q = glm::cross(s, edge1);
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

// returns -1 if no intersection, otherwise index of polygon
// sets distance to intersection point
// carries through u,v,w barycentric coordinates of intersection point
unsigned int trace(std::vector<Polygon> polygons, Ray& ray, double& distance, double& u, double& v, double& w)
{
    double closestDistance = 99999999.0;
    unsigned int closestIndex = -1;

    for (unsigned int p = 0; p < polygons.size(); p++)
    {
        if (RayIntersectsTriangle(ray, polygons[p], distance, u, v, w))
        {
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestIndex = p;
            }
        }
    }

    distance = closestDistance;
    return closestIndex;
}

// true if no obstruction along lightRay e.g. not in shadow
bool lightVisible(std::vector<Polygon> polygons, Light& light, Ray& lightRay)
{
    double t, u, v, w;

    for (auto poly : polygons)
    {
        if (RayIntersectsTriangle(lightRay, poly, t, u, v, w))
        {
            return false;
        }
    }
    return true;
}

// local light model
glm::vec3 localLight(std::vector<Polygon> polygons, unsigned int polyIndex, Light& light, AmbientLight& ambient, Ray& viewRay, glm::dvec3& intersection)
{
    Polygon poly = polygons[polyIndex];

    glm::dvec3 normal;
    if (poly.material->smoothed)
    {
        // Normale interpolieren
    }
    else
    {
        normal = poly.normal;
    }

    glm::dvec3 lightVector = glm::normalize(light.position - intersection);
    glm::dvec3 reflectVector = glm::normalize(2 * glm::dot(lightVector, normal) * normal - lightVector);
    glm::dvec3 viewVector = -1.0 * viewRay.direction;

    glm::dvec3 a, d, s;
    a = poly.material->ambient * (ambient.intensity * ambient.color);

    double theta = glm::dot(lightVector, normal);
    if (theta >= 0.0 && lightVisible(polygons, light, Ray(intersection + (0.1 * lightVector), lightVector)))
    {
        d = poly.material->diffuse * theta * (light.intensity * light.color);

        double alpha = glm::dot(reflectVector, viewVector);
        if (alpha >= 0.0)
        {
            s = poly.material->specular * glm::pow(alpha, poly.material->shininess) * (light.intensity * light.color);
            double x = glm::dot(reflectVector, viewVector);
            double y = glm::pow(x, poly.material->shininess);

            return a + d + s;
        }
        else
        {
            return a + d;
        }
    }
    else
    {
        return a;
    }
}

void rt::RaytracerSimple::render(std::shared_ptr<rt::RenderTarget> target)
{
    unsigned int height = target->getHeight();
    unsigned int width = target->getWidth();

    std::vector<glm::dvec3> vertexList{
        {-1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, 1.0, -1.0},
        {-1.0, -1.0, -1.0},
    };

    std::vector<glm::dvec3> normalList{
        { 0.0, 0.0, 1.0 },
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, -1.0 },
        { -1.0, 0.0, 0.0 },
        { 0.0, -1.0, 0.0 },
    };

    std::vector<glm::uvec3> vertexIndices {
        {1, 2, 3},
        {1, 3, 4},
        {4, 3, 5},
        {4, 5, 6},
        {4, 6, 7},
        {4, 7, 1},
        {6, 5, 8},
        {8, 8, 7},
        {1, 7, 8},
        {1, 8, 2},
        {3, 8, 5},
        {3, 2, 8},
    };

    std::vector<unsigned int> normalIndixes{
        1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    };

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->ambient = { 0.329412f, 0.223529f, 0.027451f };
    material->diffuse = { 0.780392f, 0.568627f, 0.113725f };
    material->specular = { 0.992157f, 0.941176f, 0.807843f };
    material->shininess = 27.8974f;
    material->smoothed = false;

    std::vector<Polygon> polygons = std::vector<Polygon>();

    glm::mat4 model_rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    glm::mat4 model_position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 4.0f));
    glm::mat4 model_transform = model_position * model_scale * model_rotate;
    
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        Polygon polygon = Polygon();
        polygon.v1 = model_transform * glm::vec4(vertexList[vertexIndices[i].x - 1], 1.0f);
        polygon.v2 = model_transform * glm::vec4(vertexList[vertexIndices[i].y - 1], 1.0f);
        polygon.v3 = model_transform * glm::vec4(vertexList[vertexIndices[i].z - 1], 1.0f);

        polygon.normal = normalList[normalIndixes[i] - 1];

        polygon.material = material;

        polygons.push_back(polygon);
    }

    Camera cam = Camera();
    cam.position = glm::dvec3(10.0, 3.0, 0.0);
    cam.direction = glm::normalize(glm::dvec3(0.0, 0.0, 0.0) - cam.position); // Look at {0.0, 0.0, 0.0}
    cam.height = 1.0f;
    cam.width = cam.height * ((double)width/(double)height);
    cam.imageHeight = height;
    cam.imageWidth = width;
    cam.fov = 90;
    cam.calculateDerived();

    AmbientLight ambient = AmbientLight();
    ambient.color = glm::uvec3(1.0f, 1.0f, 1.0f);
    ambient.intensity = 0.5f;

    Light light = Light();
    light.position = glm::dvec3(5.0f, 10.0f, 0.0f);
    light.color = glm::uvec3(1.0f, 1.0f, 1.0f);
    light.intensity = 1.0f;

    // ray <> triangle intersection
    double distance, u, v, w;

    for (unsigned int h = 0; h < cam.imageHeight; h++)
    {
        for (unsigned int b = 0; b < cam.imageWidth; b++)
        {
            Ray ray = Ray(cam.origin, glm::normalize(cam.centerOfPixel(h, b) - cam.origin));
            unsigned int polyIndex = trace(polygons, ray, distance, u, v, w);

            if (polyIndex != -1)
            {
                glm::dvec3 intersection = ray.origin + ray.direction * distance;
                glm::vec3 ll = localLight(polygons, polyIndex, light, ambient, ray, intersection);

                target->setPixel(b, h, ll.r, ll.g, ll.b);
            }
            else
            {
                target->setPixel(b, h, 0.5f, 0.5f, 0.5f);
            }
        }
    }
}