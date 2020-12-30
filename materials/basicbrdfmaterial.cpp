#include "basicbrdfmaterial.h"

extern bool debugFlag;

basicBRDFMaterial::basicBRDFMaterial(std::shared_ptr<texture> texture):
    materialBase(texture)
{
    type = materialType::basicBRDFMatrial;

    metallic = 0.5;
    roughness = 0.5;
}

basicBRDFMaterial::basicBRDFMaterial(std::shared_ptr<texture> texture, float m, float r):
    materialBase(texture)
{
    type = materialType::basicBRDFMatrial;

    metallic = m;
    roughness = r;
}

glm::vec3 basicBRDFMaterial::albedo(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec)
{
    glm::vec3 baseColor = tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
    glm::vec3 N = glm::normalize(hitRec.hitNormal);
    glm::vec3 L = glm::normalize(lightDirec);
    glm::vec3 V = glm::normalize(eyeDirec);
    glm::vec3 H = glm::normalize(L + V);

    NdotH = fmax(glm::dot(N, H), 0.0);
    NdotL = fmax(glm::dot(N, L), 0.0);
    NdotV = fmax(glm::dot(N, V), 0.0);
    LdotH = fmax(glm::dot(L, H), 0.0);

    F0 = glm::mix(F0, baseColor, metallic);
    float distance = hitRec.t;
    float attenuation = 1.0 / (distance * distance);

    //1.specular
    float SD = D();
    glm::vec3 SF = F(NdotL);
    float SG = G();

    glm::vec3 nom = SD * SF * SG;
    float denom = 4.0 * fmax(NdotV, 0.0) * fmax(NdotL, 0.0) + 0.001;
    glm::vec3 spec = nom / denom;

    //2.diffuse
    glm::vec3 kd = glm::vec3(1.0f) - SF;
    kd *= 1.0 - metallic;
    glm::vec3 diffuse = kd *  baseColor / PI;
    return (diffuse + spec) * attenuation * NdotL;
}

void basicBRDFMaterial::setMetallic(float m)
{
    metallic = m;
}

void basicBRDFMaterial::setRoughness(float r)
{
    roughness = r;
}

float basicBRDFMaterial::D()
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NoH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NoH2*(a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}

glm::vec3 basicBRDFMaterial::F(float a)
{
    float cosT = pow5(1.0 - a);
    return F0 + cosT * (glm::vec3(1.0f) - F0);
}

float basicBRDFMaterial::G()
{
    float ggx1 = GSub(NdotV);
    float ggx2 = GSub(NdotL);

    return ggx1*ggx2;
}

float basicBRDFMaterial::GSub(float a)
{
    float r = (roughness + 1.0);
    float k = r * r / 8.0;

    float nom = a;
    float denom = a * (1.0 - k) + k;

    return nom / denom;
}
