#include "raytracer_simple.hpp"

#include <iostream>

using namespace rt;

struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct AmbientLight
{
    glm::vec3 color;
    float intensity;
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    bool smoothed;
};

struct Polygon
{
    glm::vec3 v1, v2, v3;
    glm::vec3 normal;

    std::shared_ptr<Material> material;
};

struct Camera
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float height, width;
    unsigned int imageHeight, imageWidth;
    unsigned int fov;

    glm::vec3 origin;
    glm::vec3 topLeft;
    glm::vec3 stepPixelRight;
    glm::vec3 stepPixelDown;

    void calculateDerived()
    {
        direction = glm::normalize(direction); // normalize view direction just in case
        glm::vec3 toLeft = glm::cross(up, direction); // vector that points to the left in image plane

        // origin point of eye behind image rectangle        
        float n = -1.0 * ( (width / 2) / tan((float)fov / 2) );
        origin = position + (n * direction);
        
        // top left corner of image rectangle
        topLeft = ((width / 2) * toLeft) + ((height / 2) * up) + position; // Half width to the left, half height up from position

        // vector to describe a one pixel step to the right
        stepPixelRight = (-1.0f * (width / (float)imageWidth)) * toLeft;

        // vector to describe a one pixel step downwards
        stepPixelDown = (-1.0f * (height / (float)imageHeight)) * up;
    }

    // 0,0 at top left, y points down, x to the right
    glm::vec3 centerOfPixel(unsigned int y, unsigned int x)
    {
        return topLeft + ((float)x * stepPixelRight) + (0.5f * stepPixelRight) // x pixel to the right, starting with 0, add half a step to get to center of pixel
                       + ((float)y * stepPixelDown) + (0.5f * stepPixelDown);  // y pixel down, starting with 0, add half a step to get to center of pixel
    }
};

// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool RayIntersectsTriangle(glm::vec3 rayOrigin, glm::vec3 rayVector, Polygon inTriangle, glm::vec3* outIntersectionPoint)
{
    const float EPSILON = 0.0000001;
    glm::vec3 vertex0 = inTriangle.v1;
    glm::vec3 vertex1 = inTriangle.v2;
    glm::vec3 vertex2 = inTriangle.v3;
    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = glm::cross(rayVector, edge2);
    a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;
    f = 1 / a;
    s = rayOrigin - vertex0;
    u = f * (glm::dot(s, h));
    if (u < 0.0 || u > 1.0)
        return false;
    q = glm::cross(s, edge1);
    v = f * glm::dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        *outIntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool lightVisible(std::shared_ptr<std::vector<Polygon>> polygons, Light light, ray lightRay)
{
    glm::vec3* i = new glm::vec3();
    for (auto const poly : *polygons)
    {
        if (RayIntersectsTriangle(lightRay.origin, lightRay.direction, poly, i))
        {
            delete i;
            return false;
        }
    }
    delete i;
    return true;
}

glm::vec3 localLight(std::shared_ptr<std::vector<Polygon>> polygons, unsigned int polyIndex, Light light, AmbientLight ambient, ray viewRay, glm::vec3 intersection)
{
    Polygon poly = (*polygons)[polyIndex];

    glm::vec3 normal;
    if (poly.material->smoothed)
    {
        // Normale interpolieren
    }
    else
    {
        normal = poly.normal;
    }

    glm::vec3 lightVector = glm::normalize(light.position - intersection);
    glm::vec3 reflectVector = glm::normalize(2 * glm::dot(lightVector, normal) * normal - lightVector);
    glm::vec3 viewVector = -1.0f * viewRay.direction;

    glm::vec3 a, d, s;
    a = poly.material->ambient * (ambient.intensity * ambient.color);

    float theta = glm::dot(lightVector, normal);
    if (theta >= 0.0f && lightVisible(polygons, light, ray(intersection + (0.1f * lightVector), lightVector)))
    {
        d = poly.material->diffuse * theta * (light.intensity * light.color);

        float alpha = glm::dot(reflectVector, viewVector);
        if (alpha >= 0.0f)
        {
            s = poly.material->specular * glm::pow(alpha, poly.material->shininess) * (light.intensity * light.color);
            float x = glm::dot(reflectVector, viewVector);
            float y = glm::pow(x, poly.material->shininess);

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

std::unique_ptr<std::vector<std::vector<glm::uvec3>>> RaytracerSimple::render()
{
    unsigned int height = 1080;
    unsigned int width = 1920;

    std::unique_ptr<std::vector<std::vector<glm::uvec3>>> image = std::make_unique<std::vector<std::vector<glm::uvec3>>>(height, std::vector<glm::uvec3>(width, {128, 128, 128}));

    std::vector<glm::vec3> vertexList{
        {-1.0, 1.0, 1.0},
        {-1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, 1.0, -1.0},
        {-1.0, -1.0, -1.0},
    };

    std::vector<glm::vec3> normalList{
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

    std::shared_ptr<std::vector<Polygon>> polygons = std::make_shared<std::vector<Polygon>>(std::vector<Polygon>());

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

        polygons->push_back(polygon);
    }

    Camera cam = Camera();
    cam.position = glm::vec3(10.0f, 3.0f, 0.0f);
    cam.direction = glm::normalize(glm::vec3(0.0, 0.0, 0.0) - cam.position); // Look at {0.0, 0.0, 0.0}
    cam.height = 1.0f;
    cam.width = cam.height * ((float)width/(float)height);
    cam.imageHeight = height;
    cam.imageWidth = width;
    cam.fov = 90;
    cam.calculateDerived();

    AmbientLight ambient = AmbientLight();
    ambient.color = glm::uvec3(1.0f, 1.0f, 1.0f);
    ambient.intensity = 0.5f;

    Light light = Light();
    light.position = glm::vec3(5.0f, 10.0f, 0.0f);
    light.color = glm::uvec3(1.0f, 1.0f, 1.0f);
    light.intensity = 1.0f;

    std::vector<std::vector<ray>> rays = std::vector<std::vector<ray>>(cam.imageHeight, std::vector<ray>(cam.imageWidth, ray(glm::vec3(0.0f), glm::vec3(0.0f))));

    // generate rays
    for (unsigned int h = 0; h < cam.imageHeight; h++)
    {
        for (unsigned int w = 0; w < cam.imageWidth; w++)
        {
            rays[h][w].origin = cam.origin;
            rays[h][w].direction = glm::normalize(cam.centerOfPixel(h, w) - cam.origin);
        }
    }

    // ray <> triangle intersection
    glm::vec3* intersection = new glm::vec3(0.0);
    for (unsigned int h = 0; h < cam.imageHeight; h++)
    {
        for (unsigned int w = 0; w < cam.imageWidth; w++)
        {
            float closestDistance = 99999999.0f;
            unsigned int closestIndex = -1;

            for (unsigned int p = 0; p < polygons->size(); p++)
            {
                if (RayIntersectsTriangle(rays[h][w].origin, rays[h][w].direction, (*polygons)[p], intersection))
                {
                    float distance = glm::length(*intersection - rays[h][w].origin);
                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestIndex = p;
                    }
                }
            }

            if (closestIndex != -1)
            {
                glm::vec3 ll = localLight(polygons, closestIndex, light, ambient, rays[h][w], *intersection);
                glm::uvec3 color = glm::uvec3();

                color.r = 255 * ll.r;
                color.g = 255 * ll.g;
                color.b = 255 * ll.b;

                (*image)[h][w] = color;
            }            
        }
    }
    delete intersection;

    return image;
}