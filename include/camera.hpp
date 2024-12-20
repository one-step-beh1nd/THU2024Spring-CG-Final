#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <vecio.h>
#include <float.h>
#include <cmath>
#include <glut.h>
#include "utils.hpp"

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual void setupGLMatrix() {
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(center.x(), center.y(), center.z(),   // Position
                  center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),   // LookAt
                  up.x(), up.y(), up.z());    // Up direction
    }

    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setCenter(const Vector3f& pos) {
        this->center = pos;
    }
    Vector3f getCenter() const {
        return this->center;
    }

    void setRotation(const Matrix3f& mat) {
        this->horizontal = mat.getCol(0);
        this->up = -mat.getCol(1);
        this->direction = mat.getCol(2);
    }
    Matrix3f getRotation() const {
        return Matrix3f(this->horizontal, -this->up, this->direction);
    }

    virtual void resize(int w, int h) {
        width = w; height = h;
    }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {

    // Perspective intrinsics
    float fx;
    float fy;
    float cx;
    float cy;
    float fovyd;

public:

    float getFovy() const { return fovyd; }

    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is fovy in radian.
        fovyd = angle / 3.1415 * 180.0;
        fx = fy = (float) height / (2 * tanf(angle / 2));
        cx = width / 2.0f;
        cy = height / 2.0f;
    }

    void resize(int w, int h) override {
        fx *= (float) h / height;
        fy = fx;
        Camera::resize(w, h);
        cx = width / 2.0f;
        cy = height / 2.0f;
    }

    Ray generateRay(const Vector2f &point) override {
        float csx = (point.x() - cx) / fx;
        float csy = (point.y() - cy) / fy;
        Vector3f dir(csx, -csy, 1.0f);
        Matrix3f R(this->horizontal, -this->up, this->direction);
        dir = R * dir;
        dir = dir / dir.length();
        Ray ray(this->center, dir);
        return ray;
    }

    void setupGLMatrix() override {
        // Extrinsic.
        Camera::setupGLMatrix();
        // Perspective Intrinsic.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // field of view in Y, aspect ratio, near crop and far crop.
        gluPerspective(fovyd, cx / cy, 0.01, 100.0);
    }
};

// 参考 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 实现
class DepthCamera : public Camera{
public:
    DepthCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle, float aperture, float focalLength): Camera(center, direction, up, imgW, imgH){
        
        // this->angle = angle;
        this->focalLength = focalLength;
        this->aperture = aperture;
        this->fxy = imgH / (2 * tanf(angle / 2) * (focalLength + 1));
    }

    Ray generateRay(const Vector2f & point) override{
        Vector3f dir(point.x() - width / 2, height / 2 - point.y(), fxy * (focalLength + 1));
        dir.normalize();
        Matrix3f R(this->horizontal, -this->up, this->direction);
        dir = (R * dir).normalized();
        // dir.normalize();

        float random_angle = RND_01 * 2 * PI;
        float random_radius = RND_01 * aperture / 2;
        Vector3f delta_origin = random_radius * sin(random_angle) * up + random_radius * cos(random_angle) * horizontal;

        Vector3f final_dir = (((focalLength + 1) / Vector3f::dot(direction, dir)) * dir - delta_origin).normalized();
        // final_dir.normalize();
        return Ray(this->center + delta_origin, final_dir);
    }

protected:
    float fxy;
    float aperture;          // 光圈直径
    float focalLength;       // 焦距
};
#endif //CAMERA_H
