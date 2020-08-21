#include "pdfGenerate.h"

pdfGenerate::pdfGenerate()
{
    cPdf.reset(new cosinePdf());
}

glm::vec3 pdfGenerate::generate(const glm::vec3 & surfPos)
{
    float r = drand48();
    if(r < 0.0)
        diffuse = true;
    else
        diffuse = false;

    if(diffuse)
        return cPdf->generate(surfPos);
    else
        return lPdf->generate(surfPos);
}

float pdfGenerate::value(const glm::vec3 & direction)
{
    if(diffuse)
        return cPdf->value(direction);
    else
        return lPdf->value(direction);
}

void pdfGenerate::setLightPdf(std::shared_ptr<rectangle> rec)
{
    //default use rectangle light pdf to generate light
    lPdf.reset(new rectLightPdf(rec));
}

pdfGenerate & pdfGenerate::getInstance()
{
    static pdfGenerate instance;
    return instance;
}
