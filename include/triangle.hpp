#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
// 自主实现
class Triangle: public Object3D
{

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = a;
		this->vertices[1] = b;
		this->vertices[2] = c;
		normal = Vector3f::cross(b - a, c - a).normalized();
	}
	

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
		Vector3f E1 = vertices[0] - vertices[1];
		Vector3f E2 = vertices[0] - vertices[2];
		Vector3f S  = vertices[0] - ray.getOrigin();

		if(fabs(Vector3f::dot(normal,ray.getDirection()))<1e-6)
			return false;
		Matrix3f Rd_E1_E2(ray.getDirection(), E1, E2, true);
		Matrix3f S_E1_E2(S, E1, E2, true);
		Matrix3f Rd_S_E2(ray.getDirection(), S, E2, true);
		Matrix3f Rd_E1_S(ray.getDirection(), E1, S, true);
		float tmp = Rd_E1_E2.determinant();
		float t = S_E1_E2.determinant() / tmp;
		float beta = Rd_S_E2.determinant() / tmp;
		float gamma = Rd_E1_S.determinant() / tmp;
		if(t > 0 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1 && beta + gamma <= 1){
			if(t > tmin && t < hit.getT()){
				hit.set(t, material, normal);
				set_hitpoint_color(hit, ray.pointAtParameter(t));
				return true;
			}
		}
        return false;
	}

	void set_hitpoint_color(Hit & h, const Vector3f & position){
		if (material->hasTexture())
		{	
			float a = Vector3f::cross( vertices[1] - position, vertices[2] - position).length();
			float b = Vector3f::cross( vertices[2] - position, vertices[0] - position).length();
			float c = Vector3f::cross( vertices[0] - position, vertices[1] - position).length();
			float all = a + b + c;
			Vector2f uv = ta * (a / all) + tb * (b / all) + tc * (c / all);
			h.setColor(material->getTextureColor(uv.x(), uv.y()));
		}
		else
		{
			h.setColor(material->getColor());
		}
	}

	Vector3f normal;
	Vector3f vertices[3];
	Vector2f ta, tb, tc;
    void drawGL() override {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

protected:
};

#endif //TRIANGLE_H
