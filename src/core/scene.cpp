#include "scene.hpp"

using namespace rt;

rt::Scene::Scene() : background(fColor(0.0, 0.0, 0.0))
{
}

void Scene::transform()
{
    this->polygons.clear();

    for (int i = 0; i < this->objects.size(); i++)
    {
        Object& object = this->objects[i];

        mat4x4 model_rotate = object.getRotationMat();
        mat4x4 model_scale = object.getScaleMat();
        mat4x4 model_position = object.getTranslateMat();
        mat4x4 model_transform = model_position * model_scale * model_rotate;

        unsigned int first = this->polygons.size();

        for (Polygon& source : object.mesh.polygons)
        {
            Polygon transformed = Polygon();

            transformed.v1 = model_transform * vec4(source.v1, 1.0);
            transformed.v2 = model_transform * vec4(source.v2, 1.0);
            transformed.v3 = model_transform * vec4(source.v3, 1.0);

            transformed.n1 = model_rotate * vec4(source.n1, 1.0);
            transformed.n2 = model_rotate * vec4(source.n2, 1.0);
            transformed.n3 = model_rotate * vec4(source.n3, 1.0);

            transformed.material = object.material;
            transformed.objIndex = i;

            this->polygons.push_back(transformed);
        }

        unsigned int last = this->polygons.size() - 1;

        object.poly_first = first;
        object.poly_last = last;

        if (object.mesh.type != MeshType::noBounding)
        {
            this->setBoundingSphere(object);
        }
    }
}

vec3 Scene::getBoundingCenter(Object& obj)
{
    switch (obj.mesh.type)
    {
    case MeshType::noBounding:
    case MeshType::unityCube:
    case MeshType::unitySphere:
        return obj.position;
        break;
    }
}

floating Scene::getBoundingRadius(Object& obj)
{
    floating longest = 0.0;

    switch (obj.mesh.type)
    {
    case MeshType::noBounding:
        return 0.0;
        break;
    case MeshType::unityCube:
        for(int i = obj.poly_first; i <= obj.poly_last; i++)
        {
            floating l = glm::length(this->polygons[i].v1);
            if (l > longest)
            {
                longest = l;
            }
            l = glm::length(this->polygons[i].v2);
            if (l > longest)
            {
                longest = l;
            }
            l = glm::length(this->polygons[i].v3);
            if (l > longest)
            {
                longest = l;
            }
        }
        return longest;
        break;
    case MeshType::unitySphere:
        return glm::length(this->polygons[obj.poly_first].v1);
        break;
    }
}

void Scene::setBoundingSphere(Object& obj)
{
    obj.bounding = Sphere(getBoundingCenter(obj), getBoundingRadius(obj));
}
