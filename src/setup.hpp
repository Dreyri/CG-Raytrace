#pragma once

class Setup
{
public:
    virtual void setup() = 0;
    virtual void transform() = 0;
    virtual void render(QImage& target) = 0;
};