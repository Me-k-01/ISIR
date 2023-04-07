#include "plane_geometry.hpp"

namespace RT_ISICG
{
	
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t ) const
	{ 
		float den = dot(_normal, p_ray.getDirection());
	
		if (den == 0.f) {
			p_t = -1.f;
			return false;
		}
		p_t = dot( _point - p_ray.getOrigin(), _normal) / den;
		return true;
	}

} // namespace RT_ISICG
