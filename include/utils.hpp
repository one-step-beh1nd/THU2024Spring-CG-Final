#ifndef UTILS_HPP
#define UTILS_HPP
#include <random>
#include <cstdlib>
#include <cmath>
#include <omp.h>

/*
本文件完全拷贝
https://github.com/Guangxuan-Xiao/THU-Computer-Graphics-2020
*/

static std::mt19937 MersenneTwister;
static std::uniform_real_distribution<float> uniform;

#define RND_11 (2.0 * uniform(MersenneTwister) - 1.0) //[-1, 1]均匀分布
#define RND_01 (uniform(MersenneTwister)) // 生成[0, 1]的随机数
#define PI 3.14159265358979323846264338327950288


inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }
#endif // UTILS_HPP