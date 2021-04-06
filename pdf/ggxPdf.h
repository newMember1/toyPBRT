#ifndef GGXPDF_H
#define GGXPDF_H

#include "./core/pdfBase.h"
#include "./primitive/cube.h"

class ggxPdf :
	public pdfBase
{
public:
    ggxPdf(std::shared_ptr<cube> cubeLight)
	{
		lightCube = cubeLight;
	}

	glm::vec3 generate(const glm::vec3 & p, float roughnessA, float roughnessB) const override
	{
		glm::vec2 Xi = randomVec2();
		float a = roughnessA * roughnessA;

		float phi = 2.0 * PI * Xi.x;
		float r = 1.0 / sqrt(roughnessA * roughnessA + roughnessB * roughnessB);
		float cosTheta = roughnessA * r * sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
		float sinTheta = roughnessB * r * sqrt(1.0 - cosTheta * cosTheta);

		// from spherical coordinates to cartesian coordinates
		glm::vec3 H;
		H.x = cos(phi) * sinTheta;
		H.y = sin(phi) * sinTheta;
		H.z = cosTheta;

		// from tangent-space vector to world-space sample vector
		glm::vec3 up = abs(p.z) < 0.999f ? glm::vec3(0.0, 0.0, 1.0) : glm::vec3(1.0, 0.0, 0.0);
		glm::vec3 tangent = glm::normalize(glm::cross(up, p));
		glm::vec3 bitangent = glm::cross(p, tangent);

		glm::vec3 sampleVec = tangent * p.x + bitangent * p.y + p * H.z;
		return glm::normalize(glm::vec3(drand48(), drand48(), drand48()));
		return normalize(sampleVec);
	}

	float value(const glm::vec3 & direction) const override
	{
		return 1.0f;
	}

    lightType type() const override
    {
        return lightType::cube;
    }

private:
	glm::vec2 randomVec2() const
	{
		return glm::vec2(drand48(), drand48());
	}

	glm::vec2 hammersley(unsigned int i, unsigned int N)
	{
		return glm::vec2(float(i) / float(N), radicalInverseVdc(i));
	}

	std::shared_ptr<cube> lightCube;
};

#endif
