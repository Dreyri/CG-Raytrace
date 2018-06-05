#include "raytracer_simple.hpp"

using namespace rt;

struct Polygon
{
    glm::vec3 v1, v2, v3;
    glm::uvec3 color;
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

std::unique_ptr<std::vector<std::vector<glm::uvec3>>> RaytracerSimple::render()
{
    unsigned int height = 1080;
    unsigned int width = 1920;

    std::unique_ptr<std::vector<std::vector<glm::uvec3>>> image = std::make_unique<std::vector<std::vector<glm::uvec3>>>(height, std::vector<glm::uvec3>(width, {128, 128, 128}));

    std::vector<glm::vec3> vertexlist{
        {-1.0, 1.0, -1.0},
        {-1.0, -1.0, -1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, -1.0, 1.0},
        {-1.0, 1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0}
    };

    std::vector<glm::uvec3> indexlist {
        {0, 1, 2},
        {2, 3, 0},
        {4, 1, 0},
        {0, 5, 4},
        {2, 6, 7},
        {7, 3, 2},
        {4, 5, 7},
        {7, 6, 4},
        {0, 3, 7},
        {7, 5, 0},
        {1, 4, 2},
        {2, 4, 6}
    };

    glm::uvec3 mesh_color{ 255, 0, 0 };

    std::vector<Polygon> polygons = std::vector<Polygon>();

    glm::mat4 model_transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model_transform = glm::scale(model_transform, glm::vec3(2.0f));
    model_transform = glm::translate(model_transform, glm::vec3(0.0f, 0.0f, 0.0f));
    
    for (auto const indices : indexlist)
    {
        Polygon polygon = Polygon();
        polygon.v1 = model_transform * glm::vec4(vertexlist[indices.x], 0.0f);
        polygon.v2 = model_transform * glm::vec4(vertexlist[indices.y], 0.0f);
        polygon.v3 = model_transform * glm::vec4(vertexlist[indices.z], 0.0f);
        polygon.color = mesh_color;

        polygons.push_back(polygon);
    }

    Camera cam = Camera();
    cam.position = glm::vec3(10.0f, 0.0f, 0.0f);
    cam.direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    cam.height = 0.5f;
    cam.width = cam.height * (float)(width/height);
    cam.imageHeight = width;
    cam.imageWidth = height;
    cam.fov = 90;
    cam.calculateDerived();

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
    glm::uvec3 color;
    for (unsigned int h = 0; h < cam.imageHeight; h++)
    {
        for (unsigned int w = 0; w < cam.imageWidth; w++)
        {
            float closestDistance = 99999999.0f;
            unsigned int closestIndex = -1;

            for (unsigned int p = 0; p < polygons.size(); p++)
            {
                if (RayIntersectsTriangle(rays[h][w].origin, rays[h][w].direction, polygons[p], intersection))
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
                (*image)[w][h] = polygons[closestIndex].color;
            }            
        }
    }
    delete intersection;

    return image;
}