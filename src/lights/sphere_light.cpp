
#include "sphere_light.hpp" 
#include "utils/random.hpp" 

namespace RT_ISICG
{
	LightSample SphereLight::sample(const Vec3f& p_point) const { 
		const Vec3f posSurf = _rdmPoint();  

		const Vec3f dir = glm::normalize(posSurf - p_point); // = - wi
		const float dist = glm::distance(posSurf, p_point);
		const Vec3f normal = glm::normalize(posSurf - _center);
		const float pdf =  dist * dist / (glm::dot(normal, dir) * _area);

		const Vec3f rad = _color * _power / pdf;
		return LightSample(glm::normalize(dir), dist, rad, pdf);
	}

	Vec3f SphereLight::_rdmPoint() const {
		const float theta = TWO_PIf * randomFloat(); 
		const float phi = acos(2.f * randomFloat() - 1.f); 
		return Vec3f(
			cos(theta) * sin(phi), 
			sin(theta) * sin(phi), 
			cos(phi)
		) * _radius + _center;
	}
}