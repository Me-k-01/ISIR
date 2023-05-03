#include "sphere_light.hpp" 
#include "utils/random.hpp" 

namespace RT_ISICG
{
	LightSample SphereLight::sample(const Vec3f& p_point) const { 
		const Vec3f posSurf = _rdmPoint();  

		const Vec3f neg_wi = posSurf - p_point; // = - wi
		const Vec3f dir = glm::normalize(neg_wi);
		const float dist = glm::length(neg_wi);
		 
		Vec3f normal = glm::normalize(_center - posSurf); // Du centre vers la position sur la surface
		// On d�termine la normal suivant le sens du rayon qui a frapp� la surface 
		//normal = glm::dot( dir, normal ) > 0.f ? normal : -normal;
		
		float cosThetaI = glm::dot(dir, normal);
		if (cosThetaI < 0.f) { 
			// On peut tapper la surface depuis l'interieur de la lumi�re (pour �clairer l'objet qui emet la lumi�re)
			cosThetaI = - cosThetaI;
		}
		const float pdf = dist * dist / (cosThetaI * _area);

		const Vec3f rad = _color * _power / pdf;
		return LightSample(dir, dist, rad, pdf);
	}
	 
	Vec3f SphereLight::_rdmPoint() const { // Calcul un point al�atoire sur la sph�re distribu� de mani�re homog�ne
		const float theta = TWO_PIf * randomFloat(); 
		const float phi = acos(2.f * randomFloat() - 1.f); 
		return Vec3f(
			cos(theta) * sin(phi),  
			cos(phi),
			sin(theta) * sin(phi) 
		) * _radius + _center;
	}
}