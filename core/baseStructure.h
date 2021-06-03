/*use glm to handle operation such as rotate and transpose,may use Eigen or other libs in future*/
#ifndef BASESTRUCTURE_H
#define BASESTRUCTURE_H
#include<memory>
#include<3rdparty/glm/glm.hpp>
#include<3rdparty/glm/gtc/matrix_transform.hpp>
#include<iostream>

const float PI = 3.1415026;
const float epslion = 1e-5;

const int MAX_TRACE_TIMES = 20;
static unsigned long long seed = 1;
#define magic1 0x100000000LL
#define magic2 0xB16
#define magic3 0x5DEECE66DLL

inline void srand48(unsigned int i)
{
    seed = (((long long int)i) << 16) | rand();
}

inline double drand48(void)
{
    seed = (magic3 * seed + magic2) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return  ((double)x / (double)magic1);
}

inline float pow2(float a)
{
    return a*a;
}

inline float pow5(float a)
{
    return pow2(a)*pow2(a)*a;
}

inline float schlick(float cosine, float ref)
{
    float r0 = (1 - ref) / (1 + ref);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

//low discrepancy sequence
inline float radicalInverseVdc(unsigned int bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

//simple fog simulator
const float a = 1;
const float fogDensity = 0.008;
inline glm::vec3 applyFog(glm::vec3 rgb, float distance, glm::vec3 viewPos , glm::vec3  rayDir)
{
	float fogAmount = a * exp(-viewPos.y * fogDensity) * (1.0 - exp(-distance * rayDir.y * fogDensity)) / rayDir.y;
	glm::vec3  fogColor{ 0.5, 0.6, 0.7 };
	return glm::mix(rgb, fogColor, fogAmount);
}

//this can only be used in atmospheric scattering
const float n = 1.00029;
const float N = 2.504 * 10e25;
const float H = 8500;
const glm::vec3 lambda4{ glm::pow(glm::vec3(680 * 10e-9, 550 * 10e-9, 440 * 10e-9), glm::vec3(4.0f)) };
const glm::vec3 beta = glm::vec3(-8 * pow(PI, 3) * std::pow(n * n - 1, 2) / 3 / N) / lambda4;
inline glm::vec3 applyTransmittance(const glm::vec3 viewPos, const glm::vec3 hitPos)
{
	//only consider T(PA)
	float opticalDepth = -H * (exp(-hitPos.y / H) - exp(-viewPos.y / H));
	glm::vec3 betaD = -beta * opticalDepth;
	return glm::exp(betaD);
}

//this can be only used in atmospheric scattering
inline glm::vec3 applyRayleighScattering(const float theta, const float h)
{
    return glm::vec3( PI * PI * pow(n * n - 1, 2) / 2.0f / N * exp(-h / H) * (1 + cos(theta) * cos(theta))) / lambda4;
}

struct ray
{
    glm::vec3 pos;
    glm::vec3 direc;
    ray(const glm::vec3 &p,const glm::vec3 &d)
    {
        pos=p;
        direc=glm::normalize(d);
    }
};

struct line
{
    glm::vec3 start;
    glm::vec3 end;
    line(const glm::vec3 & s,const glm::vec3 &e)
    {
        start=s;
        end=e;
    }
    float len2()
    {
        return (start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y)+(start.z-end.z)*(start.z-end.z);
    }
    float len()
    {
        return sqrt(len2());
    }

    float len2() const
    {
        return (start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y)+(start.z-end.z)*(start.z-end.z);
    }
    float len() const
    {
        return sqrt(len2());
    }
};

enum class primitiveType
{
    triangle,
    rectangle,
	cube,
    sphere,
    unknown
};

enum class materialType
{
    simpleMaterial,
    dielectrics,
    basicBRDFMatrial,
    disneyBRDFMaterial
};

enum class colorMode
{
    iterator,
    recursive,
    hitTest,
    normalVis,
    normalTest,
    test
};

enum class lightType
{
    rectangle,
    sphere,
    cube,
    diffuse
};

#endif // BASESTRUCTURE_H
