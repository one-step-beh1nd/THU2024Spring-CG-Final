#ifndef RECURSIVE_RAY_TRACING_H
#define RECURSIVE_RAY_TRACING_H
/*
本文件独立实现
参考课程内容：Lecture 04 、习题课2
*/
#include <iostream>
#include <string.h>
#include "scene_parser.hpp"
#include "ray.hpp"
#include "group.hpp"
#include "hit.hpp"
#include "light.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "utils.hpp"
using namespace std;

void printVector3f(const Vector3f &v)
{
    std::cout << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")" << std::endl;
}

class RRT{
public:
    float weight_limit;   // 衰减阈值
    float initial_weight; // 初始权重
    float w_r;            // 反射衰减率
    float w_t;            // 投射衰减率
    int initial_depth;    // 递归次数
    int iterTime;         // 迭代次数
    SceneParser * sceneParser;  // 场景

    RRT(float _weight_limit = 0.00001,  int _initial_depth = 5, float _initial_weight = 1, int _iterTime = 1000){
        weight_limit = _weight_limit;
        w_r = 0.2;
        w_t = 0.2;
        initial_depth = _initial_depth;
        initial_weight = _initial_weight;
        iterTime = _iterTime;
    };
    
    void render(const string & inputFile, const string & outputFile){
        sceneParser = new SceneParser(inputFile.c_str());
        Camera * camera = sceneParser->getCamera();
        // SceneParser *parser = new SceneParser(inputFile.c_str());
        // Camera *camera = parser->getCamera();
        int w = camera->getWidth();
        int h = camera->getHeight();
        Image *image = new Image(w, h);

        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                // 打印进度progress
                if (i % 10 == 0 && j == 0){
                    std::cout << "progress: " << i << "/" << w << std::endl;
                }

                Vector3f color = Vector3f::ZERO;

                Ray ray = camera->generateRay(Vector2f(i, j));
                color += RayTracer(ray, initial_depth, initial_weight);
                image->SetPixel(i, j, color);

                // for(int iter = 0; iter < iterTime; iter++){
                //     Ray ray = camera->generateRay(Vector2f(i + RND_11, j + RND_11));
                //     color += RayTracer(ray, initial_depth, initial_weight);
                // }
                // image->SetPixel(i, j, color/iterTime);
            }
        }

        image->SaveBMP(outputFile.c_str());
        delete image;
    }

    Vector3f RayTracer(const Ray & ray, int depth, float weight){
        // 递归终止：给定衰减阈值
        
        if(weight < weight_limit) return Vector3f::ZERO;

        // 计算交点
        Hit P;
        bool isIntersect = sceneParser->getGroup()->intersect(ray, P, 0);

        // 手动调整交点位置，以弥补float精度问题
        P.setT(P.getT() - 0.001);

        // 如果没有交点，返回背景色
        if(!isIntersect) return sceneParser->getBackgroundColor();
        
        Vector3f finalColor = Vector3f::ZERO;
        Material * material = P.getMaterial();
        
        if(depth == 0) return finalColor;  

        Vector3f I = ray.getDirection().normalized();  // 入射方向
        Vector3f N = P.getNormal().normalized();       // 交点法向
        Vector3f S = ray.pointAtParameter(P.getT());   // 交点坐标
        float diffu = material->getType().x();
        float refl = material->getType().y();
        float refr = material->getType().z();

        if(diffu > 0){
            // TODO: shadow ray
            // 计算局部光强 from PA1
            for(int li=0; li < sceneParser->getNumLights(); li++){
                Light* light = sceneParser->getLight(li);
                Vector3f L, lightColor;
                light->getIllumination(S, L, lightColor);

                // 如果交点和光源间有阻挡，则不计算这个光源对交点的影响
                // Hit tmp;
                
                // Ray lightRay(S, L);
                // // std::cout << S << " " << L << std::endl;
                // if(sceneParser->getGroup()->intersect(lightRay, tmp, 0) == true) continue; 

                finalColor += P.getMaterial()->Shade(ray, P, L, lightColor) * diffu;
                // std::cout << "finalcolor  " << finalColor << std::endl; 
            }
        }

        // std::cout << "S:" << S << std::endl;
        // 计算镜面反射
        if(refl > 0){
            Ray reflectedRay(S, ReflectVec(I, N));
            Vector3f refColor = RayTracer(reflectedRay, depth-1, weight * w_r);
            finalColor += refColor * refl;
        }

        // 计算折射
        if(refr > 0){
            float ref_index = material->getRefractiveIndex();
            // std::cout << "I:" << I << std::endl;
            Vector3f refractdir = RefractVec(I, N, ref_index);
            Ray TransmittedRay(S + 0.1*refractdir, refractdir);
            // std::cout << TransmittedRay.getOrigin() << " " << TransmittedRay.getDirection() << std::endl;
            Vector3f transColor = RayTracer(TransmittedRay, depth-1, weight * w_t);
            // finalColor += w_t * refColor;
            // std::cout <<"transcolor" <<  transColor << std::endl;
            // std::cout << "Refractive COlor:" << material->getRefractiveColor() << std::endl;
            finalColor += transColor * refr;
            // finalColor += transColor;
        }  
        return finalColor;
    }

    Vector3f ReflectVec(const Vector3f & I, const Vector3f & N){
        Vector3f R = I - 2 * Vector3f::dot(I, N) * N;
        return R.normalized();
    }

    Vector3f RefractVec(const Vector3f & I, const Vector3f & N, float ref_index){
        // ref_index是指介质的折射率（认为介质外空气折射率为1）
        // 注：这里的I和课件中的I的方向相反
        float relative_ref_index = 1 / ref_index;
        float cosI = Vector3f::dot(I, N);
        Vector3f normal = N;
        if(cosI > 0){
            relative_ref_index = ref_index;
            normal = -N;
        }
        else{
            cosI = -cosI;   // 替换 dot(I, N) ，保证为正
        }
        float cosT_2 = 1 - relative_ref_index * relative_ref_index * (1 - cosI * cosI);

        if(cosT_2 < 0){
            // 全反射
            return ReflectVec(I, N);
        }
        Vector3f T = relative_ref_index * I + (relative_ref_index * cosI - sqrt(cosT_2)) * normal;
        // return T.normalized();

        float R0 = ((1.0 - ref_index) * (1.0 - ref_index)) / ((1.0 + ref_index) * (1.0 + ref_index));
        float schlick = R0 + (1.0 - R0) * pow(1.0 - cosI, 5);
        if(RND_01 < schlick){
            return ReflectVec(I, N);
        }else{
            return T.normalized();
        }
    }

};
#endif // RECURSIVE_RAY_TRACING_H