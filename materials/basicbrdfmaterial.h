#ifndef BASICBRDFMATERIAL_H
#define BASICBRDFMATERIAL_H
#include <vector>
#include "../core/materialBase.h"

using namespace glm;
class basicBRDFMaterial : public materialBase
{
public:
    basicBRDFMaterial(std::shared_ptr<texture> texture);
    basicBRDFMaterial(std::shared_ptr<texture> texture, float m, float r);

    void setMetallic(float m);
    void setRoughness(float r);
    glm::vec3 albedo(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec) override;
private:
	vec3 fresnelSchlick(float cosTheta, vec3 F0);

	float DistributionGGX(vec3 N, vec3 H, float roughness);
	float GeometrySchlickGGX(float NdotV, float roughness);
	float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

    float metallic = 0;
    float roughness = 0;

	std::vector<glm::vec3> lightPositions =
	{
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	std::vector<glm::vec3> lightColors =
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
};

#endif // BASICBRDFMATERIAL_H
