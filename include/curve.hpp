#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
using namespace std;


// 参考 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 实现get_closet_t
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
    float t;
    CurvePoint(Vector3f v, Vector3f tt) : V(v), T(tt) {}
};

Vector3f cur_point = Vector3f::ZERO;

bool compare(const CurvePoint &a, const CurvePoint &b)
{
    return (a.V - cur_point).squaredLength() < (b.V - cur_point).squaredLength();
}

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
    std::vector<CurvePoint> data;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void get_closest_t(const float & x, const float & y, float & t){
        cur_point = Vector3f(x, y, 0.0f);
        t = (*std::min_element(data.begin(), data.end(), compare)).t;
    }

    // 检查t是否合法
    virtual bool checkT(const float &t) = 0;
    // 返回data

    std::vector<CurvePoint> getData()
    {
        return data;
    }

    virtual void getPoint(const float &t, Vector3f &V, Vector3f &T) = 0;
    
    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};

class BezierCurve : public Curve
{
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points)
    {
        if (points.size() < 4 || points.size() % 3 != 1)
        {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        this->discretize(1000, data);
    }
    //bezier曲线的评估函数（递归定义）
    float evaluateBasis(int i, int n, float t)
    {
        if (i < 0 || i > n){
            return 0;
        }
        if (n == 0){
            return 1;
        }
        return (1 - t) * evaluateBasis(i, n - 1, t) + t * evaluateBasis(i - 1, n - 1, t);
    }
    float evaluateBasisDerivative(int i, int n, float t){
        return n * (evaluateBasis(i - 1, n - 1, t) - evaluateBasis(i, n - 1, t));
    }
    bool checkT(const float &t) override
    {
        return t >= 0.0f && t <= 1.0f;
    }

    void getPoint(const float &t, Vector3f &V, Vector3f &T) override
    {
        // 计算t的三维坐标V和导数T
        int n = controls.size() - 1;
        V = Vector3f::ZERO;
        T = Vector3f::ZERO;
        for (int i = 0; i <= n; i++)
        {
            float basis = evaluateBasis(i, n, t);
            float basisDerivative = evaluateBasisDerivative(i, n, t);
            V += basis * controls[i];
            T += basisDerivative * controls[i];
        }
    }
    void discretize(int resolution, std::vector<CurvePoint> &data) override
    {
        data.clear();
        // // TODO (PA2): fill in data vector
        resolution += 1;
        for(int st = 0;st < (controls.size()-1)/3;st++){
            vector<float> t_range;
            for(int i = 0; i < resolution; i++){
                // float t = ((float)i + st * (resolution)) / (float)((resolution) * (controls.size()-1)/3 - 1);
                float t = (float)i / (float)(resolution - 1);
                t_range.push_back(t);
            }
            for (auto t : t_range)
            {
                CurvePoint point(Vector3f::ZERO,Vector3f::ZERO);
                point.t = t;
                for (int j = 0; j < 4; j++)
                {
                    float basis = evaluateBasis(j, 3, t);
                    float basisDerivative = evaluateBasisDerivative(j, 3, t);
                    point.V += basis * controls[j+st*3];
                    point.T += basisDerivative * controls[j+st*3];
                }
                data.push_back(point);
            }
        }


        // data.clear();
        // int n = controls.size() - 1;
        // for (int i = 0; i < resolution; i++)
        // {
        //     float t = (float)i / (float)(resolution - 1);
        //     CurvePoint point(Vector3f::ZERO, Vector3f::ZERO);
        //     point.t = t;
        //     for (int j = 0; j <= n; j++)
        //     {
        //         float basis = evaluateBasis(j, n, t);
        //         float basisDerivative = evaluateBasisDerivative(j, n, t);
        //         point.V += basis * controls[j];
        //         point.T += basisDerivative * controls[j];
        //     }
        //     point.T.normalize();
        //     data.push_back(point);
        // }
    }

protected:
};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }

        this->discretize(100, data);
    }
    float evaluateBasis(int i, int k, float t, std::vector<float> knots)
    {
        if (k == 0){
            return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
        }
        else{
            return ((t - knots[i]) / (knots[i + k] - knots[i])) * evaluateBasis(i, k - 1, t, knots) + ((knots[i + k + 1] - t) / (knots[i + k + 1] - knots[i + 1])) * evaluateBasis(i + 1, k - 1, t, knots);
        }
    }
    float evaluateBasisDerivative(int i, int k, float t, std::vector<float> knots)
    {
        return k * ((evaluateBasis(i, k - 1, t, knots) / (knots[i + k] - knots[i])) - (evaluateBasis(i + 1, k - 1, t, knots) / (knots[i + k + 1] - knots[i + 1])));
    }

    bool checkT(const float &t) override
    {
        int n = controls.size() - 1;
        int k = 3;
        return (t >= k / (float)(n + k + 1)) && (t <= (n + 1) / (float)(n + k + 1));
    }

    void getPoint(const float &t, Vector3f &V, Vector3f &T) override
    {
        int n = controls.size() - 1;
        int k = 3;
        int m = n + k + 1;
        V = Vector3f::ZERO;
        T = Vector3f::ZERO;
        std::vector<float> knotVector(m + 1, 0.0f);
        for (int i = 0; i <= m; i++)
        {
            knotVector[i] = i / (float)(n + k + 1);
        }
        for (int i = 0; i <= n; i++)
        {
            float basisValue = evaluateBasis(i, k, t, knotVector);
            float basisDerivative = evaluateBasisDerivative(i, k, t, knotVector);
            V += basisValue * controls[i];
            T += basisDerivative * controls[i];
        }
    }
    void discretize(int resolution, std::vector<CurvePoint> &data) override
    {
        data.clear();
        // TODO (PA2): fill in data vector
        int n = controls.size() - 1;
        int k = 3;
        // resolution *= controls.size()-3;
        std::vector<float> knotVector(n + k + 1, 0.0f);
        for (int i = 0; i <= n + k + 1; i++)
        {
            knotVector[i] = i / (float)(n + k + 1);
        }
        for (int i = 0; i < resolution; i++)
        {
            float t = knotVector[k] + (i / (float)(resolution - 1)) * (knotVector[controls.size()] - knotVector[k]);
            CurvePoint point(Vector3f::ZERO, Vector3f::ZERO);
            point.t = t;
            for (int j = 0; j < controls.size(); j++)
            {
                float basisValue = evaluateBasis(j, 3, t, knotVector);
                float basisDerivative = evaluateBasisDerivative(j, 3, t, knotVector);
                point.V += basisValue * controls[j];
                point.T += basisDerivative * controls[j];
            }
            point.T.normalize();
            data.push_back(point);
        }
    }
};

#endif // CURVE_HPP
