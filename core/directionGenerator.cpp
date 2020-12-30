#include "directionGenerator.h"

directionGenerator::directionGenerator()
{
    this->diffusePdf.reset(new randomPdf());
}

void directionGenerator::randomChoseLight()
{
    int rnd = rand() % lightPdfs.size();
    lightPdf = lightPdfs[rnd];
}

glm::vec3 directionGenerator::generateProperDirection(const glm::vec3 & surfPos, const glm::vec3 & surfNormal)
{
    int size = lightPdfs.size();
    int start = rand() % size;
    int step = rand() % size;

    bool proper = false;
    int cnt = 0;
    while(!proper && cnt < size)
    {
        lightPdf = lightPdfs[start];
        glm::vec3 res = lightPdf->generate(surfPos);
        if(glm::dot(res, surfNormal) > 0)
            return res;

        start = (start + step) % size;
        ++cnt;
    }

    return glm::vec3(0, 0, 1);
}

glm::vec3 directionGenerator::generate(const glm::vec3 & surfPos, const glm::vec3 & surfNormal)
{

    float r = drand48();
    if(r < 0)
        diffuse = true;
    else
        diffuse = false;

    if(diffuse)
        return diffusePdf->generate(surfNormal);
    else
    {
        return generateProperDirection(surfPos, surfNormal);
    }
}

float directionGenerator::value(const glm::vec3 & direction)
{
    if(diffuse)
        return diffusePdf->value(direction);
    else
        return lightPdf->value(direction);
}

void directionGenerator::setLightPdfs(unordered_map<string, shared_ptr<pdfBase> > lights)
{
    for(auto light : lights)
        lightPdfs.push_back(light.second);
}

directionGenerator & directionGenerator::getInstance()
{
    static directionGenerator instance;
    return instance;
}
