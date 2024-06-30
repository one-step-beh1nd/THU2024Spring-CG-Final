#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"
#include <glut.h>

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(Material *material) {
        this->material = material;
    }

    virtual void set_hit_color(Hit &h, const Vector3f &position)
    {
        if (this->material->hasTexture())
        {
            printf("set_texture_color\n");
            h.setColor(Vector3f(1, 1, 1));
        }
        else
        {
            h.setColor(this->material->getColor());
        }
    }

    virtual float getArea(){
        return 0;
    }

    virtual void getPoint(Vector3f & point , Vector3f & normal){
        point = Vector3f::ZERO;
        normal = Vector3f::ZERO;
    }
    // PA1: Intersect Ray with this object. If hit, store information in hit structure.
    // This will not be used in PA2.
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    // PA2: draw using OpenGL pipeline.
    virtual void drawGL() {
        if (material) material->Use();
    }

protected:

    Material *material;
};

#endif

