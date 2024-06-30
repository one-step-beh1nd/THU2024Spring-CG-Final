#define STB_IMAGE_IMPLEMENTATION
# include <texture.hpp>
# include <stb_image.h>
# include <iostream>
using namespace std;
// 参考 https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject 实现
Texture::Texture(const char *filename)
{
    if(strlen(filename) == 0)
    {
        return;
    }
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        this->width = width;
        this->height = height;
        this->nrChannels = nrChannels;
        this->data = data;
        printf("Texture loaded: %s\n", filename);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}
Texture::~Texture()
{
    stbi_image_free(data);
}
Vector3f Texture::getColor(const Vector2f &uv) const
{
    float x = uv.x() * width;
    float y = uv.y() * height;
    int x1 = floor(x);
    int x2 = ceil(x);
    int y1 = floor(y);
    int y2 = ceil(y);
    float s = x - x1;
    float t = y - y1;
    Vector3f color = (1-s) * (1-t) * getColor(x1, y1) + (1-s) * t * getColor(x1, y2) + s * (1-t) * getColor(x2, y1) + s * t * getColor(x2, y2);
    return color;
}

Vector3f Texture::getColor(int x, int y) const
{
    
    x = max(0, min(width-1, x));
    y = max(0, min(height-1, y));
    int index = (x + y * width) * nrChannels;
    float r = data[index] / 255.0f;
    float g = data[index + 1] / 255.0f;
    float b = data[index + 2] / 255.0f;
    return Vector3f(r, g, b);
}
