#pragma once

class Setup
{
public:
    virtual void setup() = 0;
    virtual void transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animate, int slider, double index, int camHorizontal, int camVertical) = 0;
    virtual void render(QImage& target) = 0;
};