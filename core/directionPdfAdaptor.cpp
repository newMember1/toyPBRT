#include "directionPdfAdaptor.h"

extern bool debugFlag;

directionPdfAdaptor::directionPdfAdaptor()
{
}

bool directionPdfAdaptor::chooseLight()
{
    //TODO we should choose light in a proper way
    auto size = lightPdfs.size();
    if(size <= 0)
        return false;
    int rnd = rand() % lightPdfs.size();
    lightPdf = lightPdfs[rnd];
    return true;
}

glm::vec4 directionPdfAdaptor::generate(const glm::vec3 &surfPos, const glm::vec3 &surfNormal, float roughnessX, float roughnessY)
{
	if (debugFlag)
		int a = 1;
    float r = drand48();
    if(r < 0)
    {
        auto direc = diffusePdf->generate(surfPos, roughnessX, roughnessY);
        float p = diffusePdf->value(direc);
        return {direc, p};
    }
    else
    {
        if(!chooseLight())
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        if(lightPdf->type() == lightType::diffuse)
        {
            std::cout<<"ERROR, light type config not right,please check your code!";
            exit(1);
        }
        if(lightPdf->type() == lightType::cube)
        {
            auto direc = lightPdf->generate(surfNormal, roughnessX, roughnessY);
            auto p = lightPdf->value(direc);
            return {direc, p};
        }
        else
        {
            auto direc = lightPdf->generate(surfPos, roughnessX, roughnessY);
            auto p = lightPdf->value(direc);
            return {direc, p};
        }
    }
}

void directionPdfAdaptor::setLightPdfs(unordered_map<string, shared_ptr<pdfBase> > lights)
{
    for(auto light : lights)
         lightPdfs.push_back(light.second);
}

void directionPdfAdaptor::setDiffusePdfs()
{
    this->diffusePdf.reset(new randomPdf());
}

void directionPdfAdaptor::init(unordered_map<string, shared_ptr<pdfBase> > lights)
{
     if(lightPdfs.size() > 0)
         lightPdfs.clear();
     this->setLightPdfs(lights);
     this->setDiffusePdfs();
}

directionPdfAdaptor & directionPdfAdaptor::getInstance()
{
    static directionPdfAdaptor instance;
    return instance;
}
