
#include "point_light.hpp" 

namespace RT_ISICG
{
	LightSample PointLight::sample(const Vec3f& p_point) const { 
		const Vec3f direction = _position - p_point;
		const float dist = glm::length(direction);
		const Vec3f radiance = _color * (_power / (dist * dist)); 
		return LightSample(glm::normalize(direction), dist, radiance, 1.f);
	}
	
}