
#include "quad_light.hpp" 
#include "utils/random.hpp" 

namespace RT_ISICG
{
	LightSample QuadLight::sample(const Vec3f& p_point) const { 
		/*const Vec3f direction = _position - p_point;
		const float dist = glm::length(direction);
		const Vec3f radiance = _color * (_power / (dist * dist)); 
		return LightSample(glm::normalize(direction), dist, radiance, 1.f);*/

		Vec3f posSurf = _position + randomFloat() * _u + randomFloat() * _v;
		const Vec3f dir = glm::normalize(posSurf - p_point);
		const float dist = glm::distance(posSurf, p_point);
		const float pdf = (1/_area ) * ( dist * dist / glm::dot(_n, dir));
		const Vec3f rad = _color * _power / pdf;
		return LightSample(glm::normalize(dir), dist, rad, pdf);

	}
	
}