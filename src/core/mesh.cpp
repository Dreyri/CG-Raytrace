#include "mesh.hpp"

#include <unordered_map>

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

    for (unsigned int i = 0; i < 12; i++)
    {
        vertexList[i] = glm::normalize(vertexList[i]);
    }

    std::vector< glm::tvec3<uint32_t, glm::highp> > vertexIndices{
        { 0, 11, 5 },
        { 0, 5, 1 },
        { 0, 1, 7 },
        { 0, 7, 10 },
        { 0, 10, 11 },

        { 1, 5, 9 },
        { 5, 11, 4 },
        { 11, 10, 2 },
        { 10, 7, 6 },
        { 7, 1, 8 },

        { 3, 9, 4 },
        { 3, 4, 2 },
        { 3, 2, 6 },
        { 3, 6, 8 },
        { 3, 8, 9 },

        { 4, 9, 5 },
        { 2, 4, 11 },
        { 6, 2, 10 },
        { 8, 6, 7 },
        { 9, 8, 1 },
    };

    std::unordered_map<uint64_t, uint32_t> middlePointIndexCache = std::unordered_map<uint64_t, uint32_t>();
    uint32_t index = 11;

    auto getMiddlePoint = [&middlePointIndexCache, &vertexList, &index](uint32_t p1, uint32_t p2)
    {
        // first check if we have it already
        bool firstIsSmaller = p1 < p2;
        uint64_t smallerIndex = firstIsSmaller ? p1 : p2;
        uint64_t greaterIndex = firstIsSmaller ? p2 : p1;
        uint64_t key = (smallerIndex << 32) + greaterIndex;

        auto search = middlePointIndexCache.find(key);
        if (search != middlePointIndexCache.end())
        {
            return search->second;
        }

        // not in cache, calculate it
        vec3 point1 = vertexList[p1];
        vec3 point2 = vertexList[p2];
        vec3 middle = glm::normalize((point1 + point2) / 2.0);

        // add vertex
        vertexList.push_back(middle);
        index++;

        // store it, return index
        middlePointIndexCache.insert({ key, index });
        
        return index;
    };

    unsigned int recursionLevel = 2;
    for (int i = 0; i < recursionLevel; i++)
    {
        std::vector< glm::tvec3<uint32_t, glm::highp> > newIndexList = std::vector< glm::tvec3<uint32_t, glm::highp> >();

        for (auto tri : vertexIndices)
        {
            // replace triangle by 4 triangles
            uint32_t a = getMiddlePoint(tri.x, tri.y);
            uint32_t b = getMiddlePoint(tri.y, tri.z);
            uint32_t c = getMiddlePoint(tri.z, tri.x);

            newIndexList.push_back({ tri.x, a, c });
            newIndexList.push_back({ tri.y, b, a });
            newIndexList.push_back({ tri.z, c, b });
            newIndexList.push_back({ a, b, c });
        }
        
        vertexIndices = newIndexList;
    }

    Mesh mesh = Mesh();

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        Polygon polygon = Polygon();
        polygon.v1 = vertexList[vertexIndices[i].x];
        polygon.v2 = vertexList[vertexIndices[i].y];
        polygon.v3 = vertexList[vertexIndices[i].z];

        polygon.n1 = glm::normalize(polygon.v1);
        polygon.n2 = glm::normalize(polygon.v2);
        polygon.n3 = glm::normalize(polygon.v3);

        polygon.material = nullptr;

        mesh.polygons.push_back(polygon);
    }

    Mesh::unitySphere = mesh;
    return mesh;
}
