#ifndef PDFGENERATE_H
#define PDFGENERATE_H

#include <unordered_map>
#include <vector>
#include <mutex>
#include "./pdfBase.h"
#include "./pdf/cosinePdf.h"
#include "./pdf/randomPdf.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;
class directionPdfAdaptor
{
public:
    glm::vec4 generate(const glm::vec3 & surfPos, const glm::vec3 & surfNormal, float roughnessX, float roughnessY);
    static directionPdfAdaptor & getInstance();
    void init(unordered_map<string, shared_ptr<pdfBase> > lights);
private:
    bool chooseLight();
    void setLightPdfs(unordered_map<string, shared_ptr<pdfBase>> lights);
    void setDiffusePdfs();
    glm::vec3 direction(const glm::vec3 & surfPos, const glm::vec3 & surfNormal);
    float value(const glm::vec3 & direction);
    directionPdfAdaptor();

    //current working lightPdf
    std::shared_ptr<pdfBase> diffusePdf = nullptr;
    std::shared_ptr<pdfBase> lightPdf = nullptr;

    std::vector<shared_ptr<pdfBase>> lightPdfs;
    std::mutex mMutex;
};

#endif // PDFGENERATE_H
