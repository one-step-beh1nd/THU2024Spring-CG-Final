#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <glut.h>
#include "texture.hpp"

// 参考 https://raytracing.github.io/books/RayTracingInOneWeekend.html 实现type 
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0, 
    // const Vector3f & r_color = Vector3f::ZERO,
                     const Vector3f &emission = Vector3f::ZERO, const Vector3f &type = Vector3f(1, 1, 1), 
                     float refractiveIndex = 1.0f, const char *texturePath = "", int _glossy = 0, float _rough = 0.1) :
            diffuseColor(d_color), specularColor(s_color), shininess(s), emission(emission), 
            type(type), refractiveIndex(refractiveIndex), texture(texturePath){

        this->type = type / (type.x() + type.y() + type.z()); 

        // refractiveColor = r_color;

        _hasTexture = (strlen(texturePath) > 0) ? true : false;
        if(_glossy == 0) glossy = false;
        else glossy = true;
        roughness = _rough;
    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f getColor() const{
        return diffuseColor;
    }
    // 获取类型
    Vector3f getType() const
    {
        return type;
    }
    // 获取发光系数
    Vector3f getEmission() const
    {
        return emission;
    }
    // 获取折射率
    float getRefractiveIndex() const
    {
        return refractiveIndex;
    }

    // Vector3f getRefractiveColor(){
    //     return refractiveColor;
    // }

    bool hasTexture(){
        return _hasTexture;
    }

    bool getGlossy(){
        return glossy;
    }

    float getRoughness(){
        return roughness;
    }

    Vector3f getTextureColor(const Vector2f & uv){
        return texture.getColor(uv);
    }

    Vector3f getTextureColor(float u, float v){
        return texture.getColor(Vector2f(u, v));
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        
        Vector3f shaded = Vector3f::ZERO;
        Vector3f n = hit.getNormal().normalized();
        Vector3f v = -ray.getDirection().normalized();
        float diffusenum = std::max(Vector3f::dot(dirToLight,n),0.0f);
        Vector3f R = 2 * diffusenum * n - dirToLight;
        float specularnum = std::max(Vector3f::dot(v,R),0.0f);
        shaded += lightColor * (diffuseColor*diffusenum + specularColor*pow(specularnum,shininess));
        // 
        return shaded;
    }

    

    // For OpenGL, this is fully implemented
    void Use() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f));
    }

    

protected:
    Vector3f diffuseColor;   // 漫反射颜色
    Vector3f specularColor;  // 反射颜色
    float shininess;
    // Vector3f refractiveColor; // 折射颜色
    float refractiveIndex;
    Vector3f type;     // （漫反射，镜面反射, 折射）
    Vector3f emission; // 发光系数
    Texture texture;
    bool _hasTexture;
    bool glossy;
    float roughness;
};


#endif // MATERIAL_H