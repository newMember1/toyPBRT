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
    void setLightPdfs(unordered_map<string, shared_ptr<pdfBase>> lights);

    static directionGenerator & getInstance();

private:
    void randomChoseLight();
    glm::vec3 generateProperDirection(const glm::vec3 & surfPos, const glm::vec3 & surfNormal);
    directionGenerator();

    bool diffuse;
    //current working lightPdf
    std::shared_ptr<pdfBase> diffusePdf = nullptr;
    std::shared_ptr<pdfBase> lightPdf = nullptr;

    std::vector<shared_ptr<pdfBase>> lightPdfs;
};

#endif // PDFGENERATE_H
