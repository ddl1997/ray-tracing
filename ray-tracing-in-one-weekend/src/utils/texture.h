#ifndef TEXTURE_H
#define TEXTURE_H

#include "global.h"

class Texture {
public:
    virtual vec3f value(float u, float  v, const vec3f& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(vec3f c) : color_value(c) {}

    SolidColor(float red, float green, float blue)
        : SolidColor(vec3f(red, green, blue)) {}

    virtual vec3f value(float u, float v, const vec3f& p) const override {
        return color_value;
    }

private:
    vec3f color_value;
};

#endif
