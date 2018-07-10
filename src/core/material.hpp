#pragma once

#include <vector>

#include "defs.hpp"

namespace rt
{
    class Material
    {
    public:
        floating shininess;

        bool transparent;
        floating refraction_index;
        floating refraction_amount;

        floating reflection_amount;

        Material();
        virtual fColor getAmbient(vec2 uv) = 0;
        virtual fColor getDiffuse(vec2 uv) = 0;
        virtual fColor getSpecular(vec2 uv) = 0;
    };

    class SimpleMaterial : public Material
    {
    public:
        fColor ambient;
        fColor diffuse;
        fColor specular;

        SimpleMaterial();
        fColor getAmbient(vec2 uv) override;
        fColor getDiffuse(vec2 uv) override;
        fColor getSpecular(vec2 uv) override;
    };

    class Texture
    {
    public:
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual fColor getColor(int u, int v) = 0;
    };

    class Texture5x5 : public Texture
    {
    public:
        Texture5x5();
        int getWidth() override;
        int getHeight() override;
        fColor getColor(int u, int v) override;

    private:
        std::vector<std::vector<fColor>> tex;
    };

    struct TextureMaterial : public SimpleMaterial
    {
        Texture* texture;
        floating ambient;
        floating diffuse;
        floating specular;

        TextureMaterial(Texture* tex);
        fColor getAmbient(vec2 uv) override;
        fColor getDiffuse(vec2 uv) override;
        fColor getSpecular(vec2 uv) override;
    };
}
