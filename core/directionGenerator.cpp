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
	int rnd = rand() % lightPdfs.size();
	lightPdf = lightPdfs[rnd];
	return lightPdf->generate(surfPos);
}

glm::vec3 directionGenerator::generate(const glm::vec3 & surfPos, const glm::vec3 & surfNormal)
{

    float r = drand48();
    if(r < 0.5)
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
