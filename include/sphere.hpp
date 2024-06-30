#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>
#include "utils.hpp"

// 自行实现
class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
    }

    Sphere(const Vector3f &center, float radius, Material *material, Vector3f speed = Vector3f::ZERO) : Object3D(material) {
        // 
        this->center = center;
        this->radius = radius;
        this->speed = speed;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &ray, Hit &h, float tmin) override {
        // 需要计算出交点k和交点处的法向norm
        Ray r(ray.getOrigin() + RND_01 * speed, ray.getDirection());
        float t;
        Vector3f norm;
        Vector3f l = center - r.getOrigin();
        float l_2 = Vector3f::dot(l, l);
        float r_2 = radius * radius;
        float t_p = Vector3f::dot(l, r.getDirection().normalized());
        if(l_2 > r_2 && t_p < 0) return false;
        float d_2 = l_2 - t_p * t_p;
        if(d_2 > r_2) return false;
        float tt_2 = r_2 - d_2;
        if(l_2 > r_2){
            // 光源在球面外部
            t = t_p - sqrt(tt_2);
        }
        else if(l_2 < r_2){
            // 光源在球面内部
            t = t_p + sqrt(tt_2);
        }
        else{
            return false;
        }
        t /= r.getDirection().length();
        norm = r.getOrigin() + t * r.getDirection() - center;
        norm.normalize();
        if(t > tmin && t < h.getT()){
            h.set(t, material, norm);
            set_hitpoint_color(h, r.pointAtParameter(t));
            return true;
        }
        return false;
    }

    void set_hitpoint_color(Hit & h, const Vector3f & position){
        if (this->material->hasTexture())
        {
            Vector3f p = position - center;
            float u = atan2(p.x(), p.z()) / (2 * PI) + 0.5f;
            float v = 0.5 - asin(p.y() / radius) / PI; 
            h.setColor(this->material->getTextureColor(u, v));
        }
        else
        {
            h.setColor(material->getColor());
        }
    }
    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

protected:
    Vector3f center;
    float radius;
    Vector3f speed;
};


#endif
