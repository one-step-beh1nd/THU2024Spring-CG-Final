#ifndef REVSURFACE_HPP
#define REVSURFACE_HPP

#include "object3d.hpp"
#include "curve.hpp"
#include <tuple>

class RevSurface : public Object3D {

    Curve *pCurve;
    float y_min = 0.0f;
    float y_max = 0.0f;
    int iter_depth = 20;

public:
    RevSurface(Curve *pCurve, Material* material) : pCurve(pCurve), Object3D(material) {
        // Check flat.
        for (const auto &cp : pCurve->getControls()) {
            if (cp.z() != 0.0) {
                printf("Profile of revSurface must be flat on xy plane.\n");
                exit(0);
            }
            y_min = std::min(y_min, cp.y());
            y_max = std::max(y_max, cp.y());
        }
    }

    ~RevSurface() override {
        delete pCurve;
    }

// 参考实现get_f get_f_grad后发现有问题，然后自行修改 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 
    float get_f(const Vector3f &dir, const Vector3f &origin, float x, float y)
    {   
        
        float y1 = y - origin.y();
        if(abs(dir.y()) < 1e-3){
            return y1;
        }
        float a1 = pow((y1 * dir.x() + dir.y() * origin.x()), 2);
        float a2 = pow((y1 * dir.z() + dir.y() * origin.z()), 2);
        float a3 = dir.y() * dir.y() * x * x;
        return a1 + a2 - a3;
    }
    float get_f_grad(const Vector3f &dir, const Vector3f &origin, float x, float y, float x_grad, float y_grad)
    {
        float y1 = y - origin.y();
        if(abs(dir.y()) < 1e-3){
            return y_grad;
        }
        float a1 = 2 * (y1 * dir.x() + dir.y() * origin.x()) * dir.x() * y_grad;
        float a2 = 2 * (y1 * dir.z() + dir.y() * origin.z()) * dir.z() * y_grad;
        float a3 = 2 * dir.y() * dir.y() * x * x_grad;
        return a1 + a2 - a3;
    }
    

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
// 自行实现牛顿迭代
        const Vector3f d = r.getDirection();
        const Vector3f o = r.getOrigin();
        float t = 0.0f;
        Vector3f point = Vector3f::ZERO;
        Vector3f grad = Vector3f::ZERO;
        
        float t_all[10] = {0};
        bool shoulian[10] = {0};
        for(int i = 1; i < 10; i++){
            t = float(i) / float(10);

            for(int iter = 0; iter < iter_depth; iter++){
                if(!pCurve->checkT(t))
                {
                    shoulian[i] = false;
                    break;
                }
                pCurve->getPoint(t, point, grad);
                float f = get_f(d, o, point.x(), point.y());
                float f_grad = get_f_grad(d, o, point.x(), point.y(), grad.x(), grad.y());
                if (std::abs(f) < 1e-1 * d.y() * d.y()){
                    shoulian[i] = true;
                    t_all[i] = t;
                    break;
                }
                float step = f / f_grad;
                t = t - step;
            }
        }

        float tr = 99999999.0f;
        t = 99999999.0f;
        float curve_t;
        bool has_intersect = false;
        for(int i=0; i<10; i++){
            if(shoulian[i]){
                
                pCurve->getPoint(t_all[i], point, grad);
                // t = (point.y() - o.y()) / d.y();
                // has_intersect = true;

                if (abs(r.getDirection().y()) > 1e-3){
                    t = (point.y() - o.y()) / d.y();
                    has_intersect = true;
                }
                else{
                    float a = d.z() * d.z() + d.x() + d.x();
                    float b = 2 * (d.z() * o.z() + d.x() * o.x());
                    float c = o.x() * o.x() + o.z() * o.z() - point.x() * point.x();
                    float delta = b * b - 4 * a * c;
                    if(delta < 1e-3) continue;
                    float t1 = (sqrt(delta) - b) / (2 * a);
                    float t2 = (-sqrt(delta) - b) / (2 * a);
                    if(t1 > 1e-2 && t2 > 1e-2){
                        t = min(t1, t2);
                        has_intersect = true;
                    }
                    else if (t1 < 1e-2 && t2 < 1e-2){
                        
                    }
                    else{
                        t = max(t1, t2);
                        has_intersect = true;
                    }
                    // t = t1;
                    // t = t2;
                    
                }

                if(t < tr && t > 0){
                    curve_t = t_all[i];
                    tr = t;
                }
            }
        }

        if(has_intersect == false){
            return false;
        }

        tr = tr - 1e-2;
        
        pCurve->getPoint(curve_t, point, grad);

// 参考实现法向计算 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 
        Vector2f normal_2d = Vector2f(grad.y(), -grad.x()).normalized();
        Vector3f normal_3d = Vector3f::ZERO;
        Vector3f next_origin = r.pointAtParameter(tr);
        if (tr > tmin && tr < h.getT())
        {
            if (abs(point.x()) < 1e-4)
            {
                if(point.y() < 0){
                    normal_3d = Vector3f(0, -1, 0);
                }
                else{
                    normal_3d = Vector3f(0, 1, 0);
                }
            }
            else
            {
                float cosTheat = (next_origin.x()) / point.x();
                float sinTheat = (next_origin.z()) / point.x();
                normal_3d = Vector3f(cosTheat * normal_2d.x(), normal_2d.y(), sinTheat * normal_2d.x()).normalized();
            }
            h.set(tr, this->material, normal_3d);
            // std::cout << "Point:" << next_origin << "  normal:" << normal_3d << "  tr:" << tr << "  curvet:" << curve_t << std::endl;
            set_hit_color(h, next_origin);
            return true;
        }

        return false;

    }

// 参考实现 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 
    void set_hit_color(Hit &h, const Vector3f &position) override
    {
        if (this->material->hasTexture())
        {
            // float u = 0.5 + atan2(-position.z(), position.x()) / (2 * PI);
            float u = 0.5 + atan2(position.x(), position.z()) / (2 * PI);
            float v = (y_max - position.y()) / (y_max - y_min);
            Vector3f color = this->material->getTextureColor(Vector2f(u, v));
            h.setColor(color);
        }
        else
        {
            h.setColor(material->getColor());
        }
    }


    void drawGL() override {
        Object3D::drawGL();

        // Definition for drawable surface.
        typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;
        // Surface is just a struct that contains vertices, normals, and
        // faces.  VV[i] is the position of vertex i, and VN[i] is the normal
        // of vertex i.  A face is a triple i,j,k corresponding to a triangle
        // with (vertex i, normal i), (vertex j, normal j), ...
        // Currently this struct is computed every time when canvas refreshes.
        // You can store this as member function to accelerate rendering.

        struct Surface {
            std::vector<Vector3f> VV;
            std::vector<Vector3f> VN;
            std::vector<Tup3u> VF;
        } surface;

        std::vector<CurvePoint> curvePoints;
        pCurve->discretize(30, curvePoints);
        const int steps = 40;
        for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
            const CurvePoint &cp = curvePoints[ci];
            for (unsigned int i = 0; i < steps; ++i) {
                float t = (float) i / steps;
                Quat4f rot;
                rot.setAxisAngle(t * 2 * 3.14159, Vector3f::UP);
                Vector3f pnew = Matrix3f::rotation(rot) * cp.V;
                Vector3f pNormal = Vector3f::cross(cp.T, -Vector3f::FORWARD);
                Vector3f nnew = Matrix3f::rotation(rot) * pNormal;
                surface.VV.push_back(pnew);
                surface.VN.push_back(nnew);
                int i1 = (i + 1 == steps) ? 0 : i + 1;
                if (ci != curvePoints.size() - 1) {
                    surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                    surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
                }
            }
        }

        glBegin(GL_TRIANGLES);
        for (unsigned i = 0; i < surface.VF.size(); i++) {
            glNormal3fv(surface.VN[std::get<0>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<0>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<1>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<1>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<2>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<2>(surface.VF[i])]);
        }
        glEnd();
    }
};

#endif //REVSURFACE_HPP
