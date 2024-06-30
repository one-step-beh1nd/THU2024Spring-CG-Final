#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// 本文件独立实现

class Plane : public Object3D {
public:
    float axis_limit = 1e-3;
    Plane() {

    }

    Plane(const Vector3f &norm, float d, Material *m) : Object3D(m) {
        this->norm =  norm.normalized();
        this->d = d;

        U = Vector3f::ZERO;
        V = Vector3f::ZERO;
        if(norm.x() < axis_limit && norm.z() < axis_limit){
            U = Vector3f(1, 0, 0);
        }
        else{
            U = Vector3f::cross(Vector3f::UP, this->norm).normalized();
        }
        V = Vector3f::cross(this->norm, U).normalized();
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float cos = Vector3f::dot(norm,r.getDirection());
        if(fabs(cos)<1e-6)
            return false;
        
        float t = 0 - (-d + Vector3f::dot(norm, r.getOrigin())) / (Vector3f::dot(norm, r.getDirection()));
        if(t > 0 && t > tmin && t < h.getT()){
            h.set(t, material, norm);
            set_hitpoint_color(h, r.pointAtParameter(t));
            return true;
        }
        return false;
    }

    void set_hitpoint_color(Hit & hit, const Vector3f & position){
        if(material->hasTexture()){
            float u = Vector3f::dot(position - norm * d, U) / 100.0;
            float v = 1 - Vector3f::dot(position - norm * d, V) / 100.0;
            hit.setColor(material->getTextureColor(u, v));
        }
        else{
            hit.setColor(material->getDiffuseColor());
        }
    }
    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

protected:
    Vector3f norm;
    float d;
    Vector3f U;
    Vector3f V;
};

#endif //PLANE_H
		

