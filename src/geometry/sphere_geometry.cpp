#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		/// TODO !
		Vec3f oc = p_ray.getOrigin() - _center;
		// Direction normalisée donc a = 1
		// float a = dot(r.direction, r.direction);
		float b = 2.f * glm::dot(oc, p_ray.getDirection());
		float c = dot(oc, oc) - _radius * _radius;
		float delta = b * b - 4.f * c;
		if (delta >= 0) { 
			p_t1 = (-b - sqrt(delta)) / 2.f;
			p_t2 = (-b + sqrt(delta)) / 2.f;
			return true;
		} 

		p_t1 = -1.f;
		p_t2 = -1.f;
		return false;
	}

} // namespace RT_ISICG
