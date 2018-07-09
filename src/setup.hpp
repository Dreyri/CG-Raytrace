#pragma once

class Setup
{
public:
    virtual void setup() = 0;
    virtual void transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animate, int slider) = 0;
    virtual void render(QImage& target) = 0;
};