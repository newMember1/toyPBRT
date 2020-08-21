#ifndef PDFGENERATE_H
#define PDFGENERATE_H

#include <unordered_map>
#include "./pdf/cosinePdf.h"
#include "./pdf/rectLightPdf.h"

class pdfGenerate
{
public:
    glm::vec3 generate(const glm::vec3 & surfPos);
    float value(const glm::vec3 & direction);
    void setLightPdf(std::shared_ptr<rectangle> rec);

    static pdfGenerate & getInstance();

private:
    pdfGenerate();

    bool diffuse;

    std::unique_ptr<cosinePdf> cPdf;
    std::shared_ptr<rectLightPdf> lPdf;
};

#endif // PDFGENERATE_H
