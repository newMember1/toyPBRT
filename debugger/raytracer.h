#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <glm.hpp>

class rayTracer
{
public:
    void record(const glm::vec3 & point)
    {
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);
    }

    void record(float x, float y, float z)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    std::vector<float> getData()
    {
        return vertices;
    }
private:
    rayTracer();
    rayTracer & getInstance()
    {
        static rayTracer instance;
        return instance;
    }
    std::vector<float> vertices;
};

#endif // RAYTRACER_H
