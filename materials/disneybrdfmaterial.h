#ifndef DISNEYBRDF_H
#define DISNEYBRDF_H
#include"../core/materialBase.h"

class disneyBRDFMaterial:public materialBase
{
public:
    disneyBRDFMaterial(float _subSurface,float _metallic,float _specular,float _roughness,float _specularTint,float _anisotropic,float _sheen,float _sheenTint,float _clearCoat,float _clearCoatGloss,std::shared_ptr<texture> _tex);

    disneyBRDFMaterial(std::shared_ptr<texture> _tex);

    glm::vec3 albedo(const hitRecord &hitRec,const glm::vec3 & inDirec,const glm::vec3 & outDirec) override;

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


    glm::vec3 mon2lin(glm::vec3 v);
    float GTR1(float dotNH,float a);
    float GTR2(float alpha,float dotNH);
    float GTR2_aniso(float dotHX, float dotHY, float dotNH, float ax, float ay);
    float SchlickFresnel(float u);
    float smithG_GGX(float dotNV,float alphaG);
    float smithG_GGX_aniso(float dotNV,float dotVX,float dotVY,float ax,float ay);
};

#endif // DISNEYBRDF_H
