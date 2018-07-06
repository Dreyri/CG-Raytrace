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

    std::vector<glm::uvec3> normalIndixes{
        { 1, 1, 1 },
        { 1, 1, 1 },
        { 2, 2, 2 },
        { 2, 2, 2 },
        { 3, 3, 3 },
        { 3, 3, 3 },
        { 4, 4, 4 },
        { 4, 4, 4 },
        { 5, 5, 5 },
        { 5, 5, 5 },
        { 6, 6, 6 },
        { 6, 6, 6 },
    };


    Mesh mesh = Mesh();

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        Polygon polygon = Polygon();
        polygon.v1 = vertexList[vertexIndices[i].x - 1];
        polygon.v2 = vertexList[vertexIndices[i].y - 1];
        polygon.v3 = vertexList[vertexIndices[i].z - 1];

        polygon.n1 = normalList[normalIndixes[i].x - 1];
        polygon.n2 = normalList[normalIndixes[i].y - 1];
        polygon.n3 = normalList[normalIndixes[i].z - 1];

        polygon.material = nullptr;

        mesh.polygons.push_back(polygon);
    }

    Mesh::unityCube = mesh;
    return mesh;
}

Mesh Mesh::getUnitySphere()
{
    if (Mesh::unitySphere.has_value())
    {
        return Mesh::unitySphere.value();
    }

    floating t = (1.0 + glm::sqrt(5.0)) / 2.0;

    std::vector<vec3> vertexList{
        { -1.0, t, 0.0 },
        { 1.0, t, 0.0 },
        { -1.0, -t, 0.0 },
        { 1.0, -t, 0.0 },

        { 0.0, -1.0, t },
        { 0.0, 1.0, t },
        { 0.0, -1.0, -t },
        { 0.0, 1.0, -t },

        { t, 0.0, -1.0 },
        { t, 0.0, 1.0 },
        { -t, 0.0, -1.0 },
        { -t, 0.0, 1.0 },
    };

    std::vector<glm::uvec3> vertexIndices{
        { 1, 12, 6 },
        { 1, 6, 2 },
        { 1, 2, 8 },
        { 1, 8, 11 },
        { 1, 11, 12 },

        { 2, 6, 10 },
        { 6, 12, 5 },
        { 12, 11, 3 },
        { 11, 8, 7 },
        { 8, 2, 9 },

        { 4, 10, 5 },
        { 4, 5, 3 },
        { 4, 3, 7 },
        { 4, 7, 9 },
        { 4, 9, 10 },

        { 5, 10, 6 },
        { 3, 4, 12 },
        { 7, 3, 11 },
        { 9, 7, 8 },
        { 10, 9, 2 },
    };

    Mesh mesh = Mesh();

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        Polygon polygon = Polygon();
        polygon.v1 = vertexList[vertexIndices[i].x - 1];
        polygon.v2 = vertexList[vertexIndices[i].y - 1];
        polygon.v3 = vertexList[vertexIndices[i].z - 1];

        polygon.n1 = glm::normalize(polygon.v1);
        polygon.n2 = glm::normalize(polygon.v2);
        polygon.n3 = glm::normalize(polygon.v3);

        polygon.material = nullptr;

        mesh.polygons.push_back(polygon);
    }

    Mesh::unitySphere = mesh;
    return mesh;
}
