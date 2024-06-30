#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Vector2f.h"
#include "Vector3f.h"
#include "utils.hpp"


// 本文件自行实现

class Mesh : public Object3D {

public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v;
    std::vector<TriangleIndex> t;
    std::vector<Vector3f> n;
    std::vector<float> area;
    bool intersect(const Ray &r, Hit &h, float tmin) override;

    void drawGL() override {
        Object3D::drawGL();
        for (int triId = 0; triId < (int) t.size(); ++triId) {
            TriangleIndex& triIndex = t[triId];
            Triangle triangle(v[triIndex[0]],
                              v[triIndex[1]], v[triIndex[2]], material);
            triangle.normal = n[triId];
            triangle.drawGL();
        }
    }

    float getArea() override {
        float all_area = 0;
        for (float a : area) {
            all_area += a;
        }
        return all_area;
    }

    void getPoint(Vector3f& point, Vector3f& normal) override {
        float all_area = 0;
        for(float a : area){
            all_area += a;
        }

        float r = RND_01 * all_area;
        float mark = 0;
        for(int i=0; i<area.size(); i++){
            mark += area[i];
            if(mark >= r){
                float r1 = RND_01;
                float r2 = RND_01;
                float a = 1 - sqrt(r1);
                float b = (1 - r2) * sqrt(r1);
                float c = r2 * sqrt(r1);
                point = v[t[i][0]] * a + v[t[i][1]] * b + v[t[i][2]] * c;
                normal = n[i];
                return;
            }
        }
    }

private:

    // Normal can be used for light estimation
    void computeNormal();
    void computeArea();
};

#endif
