#include "raytracer_simple.hpp"

using namespace rt;

std::unique_ptr<std::vector<std::vector<glm::uvec3>>> RaytracerSimple::render()
{
    unsigned int height = 1080;
    unsigned int width = 1920;

    std::unique_ptr<std::vector<std::vector<glm::uvec3>>> image = std::make_unique<std::vector<std::vector<glm::uvec3>>>(height, std::vector<glm::uvec3>(width, {128, 128, 128}));

    return image;
}