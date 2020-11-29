#include "directionGenerator.h"

directionGenerator::directionGenerator()
{
    this->diffusePdf.reset(new randomPdf());
}

glm::vec3 directionGenerator::generate(const glm::vec3 & surfPos, const glm::vec3 & surfNormal)
{
    float r = drand48();
    if(r < 1)
        diffuse = true;
    else
        diffuse = false;

    if(diffuse)
        return diffusePdf->generate(surfNormal);
    else
        return lightPdf->generate(surfPos);
}

float directionGenerator::value(const glm::vec3 & direction)
{
    if(diffuse)
        return diffusePdf->value(direction);
    else
        return lightPdf->value(direction);
}

void directionGenerator::setLightPdf(shared_ptr<pdfBase> light)
{
    lightPdf = light;
}

void directionGenerator::setLightPdfs(unordered_map<string, shared_ptr<pdfBase> > lights)
{
    lightPdfs = lights;
}

directionGenerator & directionGenerator::getInstance()
{
    static directionGenerator instance;
    return instance;
}
