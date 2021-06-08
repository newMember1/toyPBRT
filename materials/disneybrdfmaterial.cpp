#include "disneybrdfmaterial.h"

disneyBRDFMaterial::disneyBRDFMaterial(float _subSurface,float _metallic,float _specular,float _roughness,float _specularTint,float _anisotropic,float _sheen,float _sheenTint,float _clearCoat,float _clearCoatGloss,std::shared_ptr<textureBase> _tex)
    :materialBase(_tex)
{
    type = materialType::disneyBRDFMaterial;

    subSurface=_subSurface;
    metallic=_metallic;
    specular=_specular;
    specularTint=_specularTint;
    roughness=_roughness;
    anisotropic=_anisotropic;
    sheen=_sheen;
    sheenTint=_sheenTint;
    clearCoat=_clearCoat;
    clearCoatGloss=_clearCoatGloss;

    aspect=sqrt(1-anisotropic*0.9);
    ax=glm::max(0.001f, glm::pow(roughness, 2.0f)/aspect);
    ay=glm::max(0.001f, glm::pow(roughness, 2.0f)*aspect);
}

disneyBRDFMaterial::disneyBRDFMaterial(std::shared_ptr<textureBase> _tex)
    :materialBase(_tex)
{
    type = materialType::disneyBRDFMaterial;

    subSurface=0;
    metallic=0.8;
    specular=0.8;
    specularTint=0;
    roughness=0.5;
    anisotropic=0;
    sheen=0;
    sheenTint=0.5;
    clearCoat=0;
    clearCoatGloss=1;

    aspect=sqrt(1-anisotropic*0.9);
    ax=glm::max(0.001f, glm::pow(roughness, 2.0f)/aspect);
    ay=glm::max(0.001f, glm::pow(roughness, 2.0f)*aspect);
}

void disneyBRDFMaterial::setSubSurface(float _subSurface)
{
    subSurface = _subSurface;
}

void disneyBRDFMaterial::setMetallic(float _metallic)
{
    metallic = _metallic;
}

void disneyBRDFMaterial::setSpecular(float _specular)
{
    specular = _specular;
}

void disneyBRDFMaterial::setRoughness(float _roughness)
{
    roughness = _roughness;
    aspect=sqrt(1-anisotropic*0.9);
    ax=glm::max(0.001f, glm::pow(roughness, 2.0f)/aspect);
    ay=glm::max(0.001f, glm::pow(roughness, 2.0f)*aspect);
}

void disneyBRDFMaterial::setSpecularTint(float _specularTint)
{
    specularTint = _specularTint;
}

void disneyBRDFMaterial::setAnisotropic(float _anisotropic)
{
    anisotropic = _anisotropic;
    aspect=sqrt(1-anisotropic*0.9);
    ax=glm::max(0.001f, glm::pow(roughness, 2.0f)/aspect);
    ay=glm::max(0.001f, glm::pow(roughness, 2.0f)*aspect);
}

void disneyBRDFMaterial::setSheen(float _sheen)
{
    sheen = _sheen;
}

void disneyBRDFMaterial::setSheenTint(float _sheenTint)
{
    sheenTint = _sheenTint;
}

void disneyBRDFMaterial::setClearCoat(float _clearCoat)
{
    clearCoat = _clearCoat;
}

void disneyBRDFMaterial::setClearCoatGloss(float _clearCoatGloss)
{
    clearCoatGloss = _clearCoatGloss;
}

//use two blobes here
glm::vec3 disneyBRDFMaterial::albedo(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec)
{
    N = glm::normalize(hitRec.hitNormal);
    L = glm::normalize(lightDirec);
    V = glm::normalize(eyeDirec);
    H = glm::normalize(L+V);

    dotNH = glm::dot(N,H);
    dotNL = glm::dot(N,L);
    dotNV = glm::dot(N,V);
    dotHL = glm::dot(H,L);

    if(dotNL < 0 || dotNV < 0)
        return glm::vec3(0);

    baseColor = this->tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
    Cdlin = baseColor;
    float Cdlum = 0.3 * Cdlin.x + 0.6 * Cdlin.y + 0.1 * Cdlin.z;
    Ctint = Cdlum > 0 ? Cdlin / Cdlum : glm::vec3(1.0f);
    Cspec0 = glm::mix(specular  * 0.08f * glm::mix(glm::vec3(1.0f), Ctint, specularTint), Cdlin, metallic);
    Csheen = glm::mix(glm::vec3(1.0f), Ctint,sheenTint);

    glm::vec3 diffColor = diff();
    glm::vec3 specColor = spec(hitRec.xAxis, hitRec.yAxis);
    return specColor + diffColor;
}

glm::vec3 disneyBRDFMaterial::diff()
{
    float FL = SchlickFresnel(dotNL), FV = SchlickFresnel(dotNV);
    float Fd90 = 0.5 + 2 * dotHL * dotHL * roughness;
    float Fd = glm::mix(1.0f, Fd90, FL) * glm::mix(1.0f, Fd90, FV);

    float Fss90 = dotHL * dotHL * roughness;
    float Fss = glm::mix(1.0f, Fss90, FL) * glm::mix(1.0f, Fss90, FV);
    float ss = 1.25 * (Fss * (1.0 / (dotNL + dotNV) - 0.5) + 0.5);

    return (1.0f / PI) * glm::mix(Fd, ss, subSurface) * Cdlin * (1 -  metallic);
}

glm::vec3 disneyBRDFMaterial::spec(glm::vec3 X, glm::vec3 Y)
{
    float Ds = GTR2_aniso(dotNH, glm::dot(H, X), glm::dot(H, Y), ax, ay);
    float FH = SchlickFresnel(dotHL);
    glm::vec3 Fs = mix(Cspec0, glm::vec3(1), FH);
    float Gs;

    Gs  = smithG_GGX_aniso(dotNL, glm::dot(L, X), glm::dot(L, Y), ax, ay);
    Gs *= smithG_GGX_aniso(dotNV, glm::dot(V, X), glm::dot(V, Y), ax, ay);

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = GTR1(dotNH, glm::mix(0.1f, 0.001f, clearCoatGloss));
    float Fr = glm::mix(.04, 1.0, FH);
    float Gr = smithG_GGX(dotNL, .25) * smithG_GGX(dotNV, .25);

    return Gs * Fs * Ds + 0.25f * clearCoat * Gr * Fr * Dr;
}

glm::vec3 disneyBRDFMaterial::mon2lin(glm::vec3 v)
{
    return glm::vec3(pow(v.x,2.2),pow(v.y,2.2),pow(v.z,2.2));
}

float disneyBRDFMaterial::GTR1(float dotNH,float a)
{
    if(a >= 1) return 1.0 / PI;
    float a2 = a * a;
    float t = 1 + (a2 - 1) * dotNH * dotNH;
    return (a2 - 1) / (PI * log(a2) * t);
}

float disneyBRDFMaterial::GTR2(float alpha,float dotNH)
{
    float a2 = alpha * alpha;
    float cos2th = dotNH * dotNH;
    float den = (1.0 + (a2 - 1.0) * cos2th);

    return a2 / (PI * den * den);
}

float disneyBRDFMaterial::GTR2_aniso(float dotNH, float dotHX, float dotHY, float ax, float ay)
{
    float deno = glm::pow(dotHX / ax, 2) + glm::pow(dotHY / ay, 2) + glm::pow(dotNH, 2);
    return 1.0 / (PI * ax * ay * deno * deno);
}

float disneyBRDFMaterial::SchlickFresnel(float u)
{
    float m = glm::clamp(1 - u, 0.0f, 1.0f);
    return pow5(m);
}

float disneyBRDFMaterial::smithG_GGX(float dotNV,float alphaG)
{
    float a = alphaG * alphaG;
    float b = dotNV * dotNV;
    return 1.0 / (dotNV + sqrt(a + b - a * b));
}

float disneyBRDFMaterial::smithG_GGX_aniso(float dotNV,float dotVX,float dotVY,float ax,float ay)
{
    return 1.0 / (dotNV + sqrt(pow(dotVX * ax, 2.0) + pow(dotVY * ay, 2.0) + pow(dotNV, 2.0)));
}

