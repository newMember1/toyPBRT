#ifndef DISNEYBRDF_H
#define DISNEYBRDF_H
#include "../core/materialBase.h"
#include "../core/pdfBase.h"

class disneyBRDFMaterial :public materialBase
{
public:
    disneyBRDFMaterial(float _subSurface,float _metallic,float _specular,float _roughness,float _specularTint,float _anisotropic,float _sheen,float _sheenTint,float _clearCoat,float _clearCoatGloss,std::shared_ptr<texture> _tex);

    disneyBRDFMaterial(std::shared_ptr<texture> _tex);

    glm::vec3 albedo(const hitRecord &hitRec,const glm::vec3 & lightDirec,const glm::vec3 & eyeDirec) override;

    glm::vec3 diff();

    glm::vec3 spec(glm::vec3 X, glm::vec3 Y);

    void setSubSurface(float _subSurface);

    void setMetallic(float _metallic);

    void setSpecular(float _specular);

    void setRoughness(float _roughness);

    void setSpecularTint(float _specularTint);

    void setAnisotropic(float _anisotropic);

    void setSheen(float _sheen);

    void setSheenTint(float _sheenTint);

    void setClearCoat(float _clearCoat);

    void setClearCoatGloss(float _clearCoatGloss);
private:
    glm::vec3 baseColor;
    float subSurface;
    float metallic;
    float specular;
    float specularTint;
    float roughness;
    float anisotropic;
    float sheen;
    float sheenTint;
    float clearCoat;
    float clearCoatGloss;

    float aspect;
    float ax;
    float ay;

    glm::vec3 mon2lin(glm::vec3 v);
    float GTR1(float dotNH,float a);
    float GTR2(float alpha,float dotNH);
    float GTR2_aniso(float dotNH, float dotHX, float dotHY, float ax, float ay);
    float SchlickFresnel(float u);
    float smithG_GGX(float dotNV,float alphaG);
    float smithG_GGX_aniso(float dotNV,float dotVX,float dotVY,float ax,float ay);

    glm::vec3 N;
    glm::vec3 L;
    glm::vec3 V;
    glm::vec3 H;
    float dotNH;
    float dotNL;
    float dotNV;
    float dotHL;

    glm::vec3 Cdlin;
    glm::vec3 Ctint;
    glm::vec3 Cspec0;
    glm::vec3 Csheen;
};

#endif // DISNEYBRDF_H
