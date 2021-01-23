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

glm::vec3 basicBRDFMaterial::albedo4Lights(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec)
{
	// ref:learnopengl-cn : radiance
	vec3 baseColor = tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
	vec3 N = normalize(hitRec.hitNormal);
	vec3 V = normalize(eyeDirec);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, baseColor, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < 4; ++i)
	{
		// calculate per-light radiance
		vec3 L = normalize(lightPositions[i] - hitRec.hitPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - hitRec.hitPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = vec3(attenuation);

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
		vec3 specular = nominator / max(denominator, 0.001f); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0f);

		// add to outgoing radiance Lo
		Lo += (kD * baseColor / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}
	vec3 color =  Lo;
	return color;
}

glm::vec3 basicBRDFMaterial::albedo(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec)
{
	return albedo4Lights(hitRec, lightDirec, eyeDirec);
	vec3 baseColor = tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
	vec3 N = normalize(hitRec.hitNormal);
	vec3 V = normalize(eyeDirec);
	vec3 L = normalize(lightDirec);
	vec3 H = normalize(V + L);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, baseColor, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);

	// calculate per-light radiance
	float distance = hitRec.t;
	float attenuation = 1.0 / (distance * distance) / 6;
	vec3 radiance = vec3(attenuation);

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0f, 1.0f), F0);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
	vec3 specular = nominator / max(denominator, 0.001f); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

	// kS is equal to Fresnel
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0f);

	// add to outgoing radiance Lo
	return (kD * baseColor / PI + specular) * radiance * NdotL;
}

void basicBRDFMaterial::setMetallic(float m)
{
    metallic = m;
}

void basicBRDFMaterial::setRoughness(float r)
{
    roughness = r;
}

vec3 basicBRDFMaterial::fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(max(1.0f - cosTheta, 0.0f), 5.0f);
}

float basicBRDFMaterial::DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.0000001f); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float basicBRDFMaterial::GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// ----------------------------------------------------------------------------
float basicBRDFMaterial::GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}
