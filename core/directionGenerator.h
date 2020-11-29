#ifndef PDFGENERATE_H
#define PDFGENERATE_H

#include <unordered_map>
#include <vector>
#include "./pdfBase.h"
#include "./pdf/cosinePdf.h"
#include "./pdf/randomPdf.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
class directionGenerator
{
public:
    glm::vec3 generate(const glm::vec3 & surfPos, const glm::vec3 & surfNormal);
    float value(const glm::vec3 & direction);
    void setLightPdf(shared_ptr<pdfBase> light);
    void setLightPdfs(unordered_map<string, shared_ptr<pdfBase>> lights);

    static directionGenerator & getInstance();

private:
    directionGenerator();

    bool diffuse;
    std::unique_ptr<pdfBase> diffusePdf;
    std::shared_ptr<pdfBase> lightPdf;

    unordered_map<string, shared_ptr<pdfBase>> diffusePdfs;
    unordered_map<string, shared_ptr<pdfBase>> lightPdfs;
};

#endif // PDFGENERATE_H
