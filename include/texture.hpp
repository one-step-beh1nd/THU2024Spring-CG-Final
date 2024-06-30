#ifndef TEXTURE_H
#define TEXTURE_H

#include <vecmath.h>
#include <cstring>
// texture mapping 
// 参考 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 实现
class Texture{
private:
    int width;
    int height;
    int nrChannels;
    unsigned char *data;

public:
    Texture() = default;
    Texture(const char *filename);
    ~Texture();
    Vector3f getColor(const Vector2f &uv) const;
    Vector3f getColor(int x, int y) const;
};

#endif // TEXTURE_H