#include "mesh.hpp"

using namespace rt;

std::optional<Mesh> Mesh::unityCube = std::nullopt;
std::optional<Mesh> Mesh::unitySphere = std::nullopt;

Mesh Mesh::getUnityCube()
{
    if (Mesh::unityCube.has_value())
    {
        return Mesh::unityCube.value();
    }
    
    std::vector<vec3> vertexList{
        { -1.0, 1.0, 1.0 },
        { -1.0, -1.0, 1.0 },
        { 1.0, -1.0, 1.0 },
        { 1.0, 1.0, 1.0 },
        { 1.0, -1.0, -1.0 },
        { 1.0, 1.0, -1.0 },
        { -1.0, 1.0, -1.0 },
        { -1.0, -1.0, -1.0 },
    };

    std::vector<vec3> normalList{
        { 0.0, 0.0, 1.0 },
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
        { 0.0, 0.0, -1.0 },
        { -1.0, 0.0, 0.0 },
        { 0.0, -1.0, 0.0 },
    };

    std::vector<glm::uvec3> vertexIndices{
        { 1, 2, 3 },
        { 1, 3, 4 },
        { 4, 3, 5 },
        { 4, 5, 6 },
        { 4, 6, 7 },
        { 4, 7, 1 },
        { 6, 5, 8 },
        { 8, 8, 7 },
        { 1, 7, 8 },
        { 1, 8, 2 },
        { 3, 8, 5 },
        { 3, 2, 8 },
    };

    std::vector<unsigned int> normalIndixes{
        1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    };


    Mesh mesh = Mesh();

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        Polygon polygon = Polygon();
        polygon.v1 = vertexList[vertexIndices[i].x - 1];
        polygon.v2 = vertexList[vertexIndices[i].y - 1];
        polygon.v3 = vertexList[vertexIndices[i].z - 1];

        polygon.normal = normalList[normalIndixes[i] - 1];

        polygon.material = nullptr;

        mesh.polygons.push_back(polygon);
    }

    Mesh::unityCube = mesh;
    return mesh;
}

Mesh Mesh::getUnitySphere()
{
    return Mesh();
}
