#include "scene.hpp"

using namespace rt;

void Scene::transform()
{
    this->polygons.clear();

    for (Object& object : this->objects)
    {
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

            this->polygons.push_back(transformed);
        }

        unsigned int last = this->polygons.size() - 1;

        object.poly_first = first;
        object.poly_last = last;
    }

    this->camera.calculateDerived();
}
