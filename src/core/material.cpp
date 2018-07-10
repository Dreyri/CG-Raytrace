#include "material.hpp"

using namespace rt;

SimpleMaterial::SimpleMaterial()
{
}

fColor SimpleMaterial::getAmbient(vec2 uv)
{
    return ambient;
}

fColor rt::SimpleMaterial::getDiffuse(vec2 uv)
{
    return diffuse;
}

fColor rt::SimpleMaterial::getSpecular(vec2 uv)
{
    return specular;
}

rt::TextureMaterial::TextureMaterial(Texture* tex) : texture{tex}
{
}

int toIndex(floating coord, int max)
{
    return static_cast<int>(std::trunc(((coord > 0.999999) ? 0.999999 : coord) * max));
}

fColor rt::TextureMaterial::getAmbient(vec2 uv)
{
    int u = toIndex(uv.x, texture->getWidth());
    int v = toIndex(uv.y, texture->getHeight());
    return texture->getColor(u, v) * ambient;
}

fColor rt::TextureMaterial::getDiffuse(vec2 uv)
{
    int u = toIndex(uv.x, texture->getWidth());
    int v = toIndex(uv.y, texture->getHeight());
    return texture->getColor(u, v) * diffuse;
}

fColor rt::TextureMaterial::getSpecular(vec2 uv)
{
    int u = toIndex(uv.x, texture->getWidth());
    int v = toIndex(uv.y, texture->getHeight());
    return texture->getColor(u, v) * specular;
}

rt::Texture5x5::Texture5x5()
{
    tex = { 
        { fColor(0.2), fColor(0.8), fColor(0.2), fColor(0.8), fColor(0.2), },
        { fColor(0.8), fColor(0.2), fColor(0.8), fColor(0.2), fColor(0.8), },
        { fColor(0.2), fColor(0.8), fColor(0.2), fColor(0.8), fColor(0.2), },
        { fColor(0.8), fColor(0.2), fColor(0.8), fColor(0.2), fColor(0.8), },
        { fColor(0.2), fColor(0.8), fColor(0.2), fColor(0.8), fColor(0.2), },
    };
}

int rt::Texture5x5::getWidth()
{
    return 5;
}

int rt::Texture5x5::getHeight()
{
    return 5;
}

fColor rt::Texture5x5::getColor(int u, int v)
{
    return tex[getHeight() - 1 - u][v];
}

rt::Material::Material() : transparent{ false }, reflection_amount{ 0.5 }, refraction_amount{ 0.5 }
{
}
