#ifndef PATH_TRACING_H
#define PATH_TRACING_H
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

class PT{
public:
    int samplesPerPixel;
    SceneParser * sceneParser;

    PT(int _iterTime = 1000){
        samplesPerPixel = _iterTime;
    }

    void render(const string & inputFile, const string & outputFile){
        // std::cout << "here read file" << std::endl;
        sceneParser = new SceneParser(inputFile.c_str());
        // std::cout << "here read file" << std::endl;
        Camera * camera = sceneParser->getCamera();
        int w = camera->getWidth();
        int h = camera->getHeight();
        Image * img = new Image(w, h);

#pragma omp parallel for schedule(dynamic, 1)

        for(int i = 0; i < w; i++){
            fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samplesPerPixel, 100. * i / (w - 1));
            for(int j = 0; j < h; j++){
                Vector3f color = Vector3f::ZERO;
                // std::cout << i << " " << j << std::endl;
                for(int k = 0; k < samplesPerPixel; k++){
                    // std::cout << k << "sample " << std::endl;
                    Ray camRay = camera->generateRay(Vector2f(i + RND_11, j + RND_11));
                    color += pathTracer(camRay, 0);
                }
                img->SetPixel(i, j, (color / samplesPerPixel));

// 抗锯齿：参考smallpt实现
                // for(int sy = 0; sy < 2; sy++){
                //     for(int sx = 0; sx < 2; sx++){
                //         Vector3f r = Vector3f::ZERO;
                //         for(int k=0; k < samplesPerPixel; k++){
                //             Ray camRay = camera->generateRay(Vector2f(i + (RND_11 + sx + 0.5) / 2, j + (RND_11 + sy + 0.5)/2));
                //             r += pathTracer(camRay, 0);
                //         }
                //         r = r / samplesPerPixel;
                //         color = color + Vector3f(clamp(0, 1, r.x()), clamp(0, 1, r.y()), clamp(0, 1, r.z()));
                //     }
                // }
                // color = color / 4;
                // img->SetPixel(i, j, color);
            }
        }

        img->SaveBMP(outputFile.c_str());
        delete img;
    }

// 参考smallpt实现 pathtracer NEE
    Vector3f pathTracer(const Ray & ray, int depth, int E = 1){
        Hit hit;
        bool isIntersect = sceneParser->getGroup()->intersect(ray, hit, 1e-3);
        if(!isIntersect) return Vector3f::ZERO;

        Vector3f I = ray.getDirection().normalized();  // 入射方向
        Vector3f S = ray.pointAtParameter(hit.getT());                 // 交点坐标
        Vector3f N = hit.getNormal().normalized();                               // 交点法向
        Vector3f hit_color = hit.getColor();                                 // 实现纹理
        Vector3f emission = hit.getMaterial()->getEmission();                // 交点发光项
        Vector3f type = hit.getMaterial()->getType();                        // 交点材质类型

        float P_RR = 0.9;
        // Russian Roulette
        if(depth > 5){
                // 0.1的概率停止
            if(RND_01 > 0.9){
                return Vector3f::ZERO;
                // return emission;
            }
            hit_color = hit_color * (1.0 / P_RR);
        }

        float tmp = RND_01;
        if(tmp < type.x()){
            // diffuse
            Ray newRay(S, DiffuseVec(I, N));
            if (hit.getMaterial()->getGlossy()) {
                hit_color = glossy(newRay, ray, hit) * (1.0 / P_RR);
            }
// no NEE   
            return emission + hit_color * pathTracer(newRay, depth+1);
        }
        else if(tmp < type.x() + type.y()){
            // specular
            Ray newRay(S, ReflectVec(I, N));
            return emission + hit_color * pathTracer(newRay, depth+1);
        }
        else{
            // refract
            float refr = hit.getMaterial()->getRefractiveIndex();
            Ray newRay(S, RefractVec(I, N, refr));
            // hit_color = glossy(newRay, ray, hit);
            return emission + hit_color * pathTracer(newRay, depth+1);
        }
    }

// 独立实现glossy
    Vector3f glossy(const Ray & newRay, const Ray & viewRay, const Hit & hit){

        Vector3f rho = hit.getMaterial()->getDiffuseColor(); 
        float d = 1;    // 漫反射系数
        float s = 20;    // 镜面反射系数
        Vector3f l = -viewRay.getDirection().normalized();      // 光线方向
        // Vector3f l = newRay.getDirection().normalized();
        // Vector3f v = (sceneParser->getCamera()->getCenter() - viewRay.pointAtParameter(hit.getT())).normalized();     // 观察方向
        Vector3f v = newRay.getDirection().normalized();
        Vector3f n = hit.getNormal().normalized();
        Vector3f h = (v + l).normalized();              // 角平分线
        float roughness = hit.getMaterial()->getRoughness();

        Vector3f diffuse = d * rho / PI;
        // diffuse = Vector3f::ZERO;

        float F = Schlick(v, n, hit.getMaterial()->getRefractiveIndex());
        float D = D_GGX(n, h, roughness);

        float vh = abs(Vector3f::dot(v, h));
        float nh = abs(Vector3f::dot(n, h));
        float nv = abs(Vector3f::dot(n, v));
        float nl = abs(Vector3f::dot(n, l));

        float tmp1 = 2 * nh * nv / vh;
        float tmp2 = 2 * nh * nl / vh;


        float k = (roughness + 1) * (roughness + 1) / 8;
        float G = 1 / ((nv * (1-k) + k) * (nl * (1-k) + k));
        float specular = s * F * D * G / (4 * PI);
        Vector3f lamda = Vector3f(clamp(0, 1, diffuse.x() + specular), clamp(0, 1, diffuse.y() + specular), clamp(0, 1, diffuse.z() + specular));
        return lamda;
    }

// 参考 https://zhuanlan.zhihu.com/p/160804623 实现法向分布函数
    float D_GGX(const Vector3f & N, const Vector3f & H, float a){
        float a2 = a * a;
        float NdotH = Vector3f::dot(N, H);
        if(NdotH < 0) NdotH = 0;
        float NdotH2 = NdotH * NdotH;
        float nom = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = PI * denom * denom;
        return nom / denom;
    }
// 参考 https://zhuanlan.zhihu.com/p/160804623 实现菲涅尔近似
    float Schlick(const Vector3f & V, const Vector3f & N, float ref_index){
        float cosI = abs(Vector3f::dot(V, N));
        float F0 = 0.2;
        float schlick = F0 + (1.0 - F0) * pow(1.0 - cosI, 5);
        return schlick;
    }

    
/*
独立实现 ReflectVec, RefractVec
参考课程内容：Lecture 04
*/
    Vector3f ReflectVec(const Vector3f & I, const Vector3f & N){
        // 理想反射
        Vector3f R = I - 2 * Vector3f::dot(I, N) * N;
        return R.normalized();
    }

    Vector3f RefractVec(const Vector3f & I, const Vector3f & N, float ref_index){
        // 理想折射
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
        schlick = 0.25 + 0.5 * schlick;
        if(RND_01 < schlick){
            return ReflectVec(I, N);
        }else{
            return T.normalized();
        }
    }

// cos-weighted采样：参考smallpt实现
    Vector3f DiffuseVec(const Vector3f & I, const Vector3f & N){
        // cos-weighted
        float r1 = 2 * PI * RND_01;
        float r2 = RND_01;
        float r2s = sqrt(r2);
        Vector3f w = Vector3f::dot(I, N) < 0 ? N : -N;
        Vector3f u = Vector3f::cross((std::abs(w.x()) > 0.1 ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0)), w).normalized();
        Vector3f v = Vector3f::cross(w, u);
        Vector3f d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2)).normalized();
        return d;
    }
};
#endif