#include "disneybrdfmaterial.h"

disneyBRDFMaterial::disneyBRDFMaterial(float _subSurface,float _metallic,float _specular,float _roughness,float _specularTint,float _anisotropic,float _sheen,float _sheenTint,float _clearCoat,float _clearCoatGloss,std::shared_ptr<texture> _tex)
    :materialBase(_tex)
{
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
}

disneyBRDFMaterial::disneyBRDFMaterial(std::shared_ptr<texture> _tex)
    :materialBase(_tex)
{
    subSurface=0;
    metallic=0;
    specular=0.5;
    specularTint=0;
    roughness=0.5;
    anisotropic=0;
    sheen=0;
    sheenTint=0.5;
    clearCoat=0;
    clearCoatGloss=1;
}

glm::vec3 disneyBRDFMaterial::albedo(const hitRecord &hitRec,const glm::vec3 & inDirec,const glm::vec3 & outDirec)
{
    glm::vec3 N=glm::normalize(hitRec.hitNormal);
    glm::vec3 L=glm::normalize(inDirec);
    glm::vec3 V=glm::normalize(outDirec);
    glm::vec3 H=glm::normalize(L+V);

    float dotNH=glm::dot(N,H);
    float dotNL=glm::dot(N,L);
    float dotNV=glm::dot(N,V);
    float dotHL=glm::dot(H,L);
//    if(dotNL<0||dotNV<0)
//    {
//        std::cout<<"N is: "<<N.x<<" "<<N.y<<" "<<N.z<<std::endl;
//        std::cout<<"L is: "<<L.x<<" "<<L.y<<" "<<L.z<<std::endl;
//        std::cout<<"V is: "<<V.x<<" "<<V.y<<" "<<V.z<<std::endl;
//    }

    baseColor=this->tex->baseColor(hitRec.u,hitRec.v,hitRec.hitPos);
    glm::vec3 Cdlin=mon2lin(baseColor);
    float Cdlum=0.3*Cdlin.x+0.6*Cdlin.y+0.1*Cdlin.z;
    glm::vec3 Ctint=Cdlum>0?Cdlin/Cdlum:glm::vec3(1.0f);
    glm::vec3 Cspec0=glm::mix(specular*0.08f*glm::mix(glm::vec3(1.0f),Ctint,specularTint),Cdlin,metallic);
    glm::vec3 Csheen=glm::mix(glm::vec3(1.0f),Ctint,sheenTint);

    //1.diffuse item,include surface diffuse and subsurface scattering
    float FL=SchlickFresnel(dotNL),FV=SchlickFresnel(dotNV);
    float Fd90=0.5+2*dotHL*roughness;
    float Fd=glm::mix(1.0f,Fd90,FL)*glm::mix(1.0f,Fd90,FV);

    float Fss90=dotHL*dotHL*roughness;
    float Fss=glm::mix(1.0f,Fss90,FL)*glm::mix(1.0f,Fss90,FV);
    float ss=1.25*(Fss*(1.0/(dotNL+dotNV)-0.5)+0.5);

    //2.then specular
    float aspect=sqrt(1-anisotropic*0.9);
    float ax=glm::max(0.001f,pow2(roughness)/aspect);
    float ay=glm::max(0.001f,pow2(roughness)*aspect);

    float dotHX=glm::dot(H,hitRec.xAxis);
    float dotHY=glm::dot(H,hitRec.yAxis);
    float Ds=GTR2_aniso(dotNH,dotHX,dotHY,ax,ay);
    float FH=SchlickFresnel(dotHL);
    glm::vec3 Fs=glm::mix(Cspec0,glm::vec3(1.0f),FH);
    float Gs;
    Gs=smithG_GGX_aniso(dotNL,glm::dot(L,hitRec.xAxis),glm::dot(L,hitRec.yAxis),ax,ay);
    Gs*=smithG_GGX_aniso(dotNV,glm::dot(V,hitRec.xAxis),glm::dot(V,hitRec.yAxis),ax,ay);

    //sheen
    glm::vec3 Fsheen=FH*sheen*Csheen;
    //clearcoat
    float Dr=GTR1(dotNH,glm::mix(0.1,0.001,clearCoatGloss));
    float Fr=glm::mix(0.04,1.0,FH);
    float Gr=smithG_GGX(dotNL,0.25)*smithG_GGX(dotNV,0.25);

    return ((1.0f/PI)*glm::mix(Fd,ss,subSurface)*Cdlin+Fsheen)
            *(1-metallic)
            +Gs*Fs*Ds+0.25f*clearCoat*Gr*Fr*Dr;
}

glm::vec3 disneyBRDFMaterial::mon2lin(glm::vec3 v)
{
    return glm::vec3(pow(v.x,2.2),pow(v.y,2.2),pow(v.z,2.2));
}

float disneyBRDFMaterial::GTR1(float dotNH,float a)
{
    if(a>=1) return 1.0/PI;
    float a2=a*a;
    float t=1+(a2-1)*dotNH*dotNH;
    return (a2-1)/(PI*log(a2)*t);
}

float disneyBRDFMaterial::GTR2(float alpha,float dotNH)
{
    float a2 = alpha * alpha;
    float cos2th = dotNH * dotNH;
    float den = (1.0 + (a2 - 1.0) * cos2th);

    return a2 / (PI * den * den);
}

float disneyBRDFMaterial::GTR2_aniso(float dotHX, float dotHY, float dotNH, float ax, float ay)
{
    float deno = dotHX * dotHX / (ax * ax) + dotHY * dotHY / (ay * ay) + dotNH * dotNH;
    return 1.0 / (PI * ax * ay * deno * deno);
}

float disneyBRDFMaterial::SchlickFresnel(float u)
{
    float m = glm::clamp(1-u,0.0f,1.0f);
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
