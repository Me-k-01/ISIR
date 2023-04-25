#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		/// SphereTracing  
		float distance = p_tMin;
		Vec3f position = p_ray.getOrigin() + distance * p_ray.getDirection();
		float d = _sdf(position);
		// Tant que l'on est pas suffisament proche de la surface
		while (d > _minDistance) { 
			distance += d;
			if (distance > p_tMax) {
				return false;
			}
			position = p_ray.getOrigin() + distance * p_ray.getDirection();
			const float nextD = _sdf(position);
			// Lorsque l'on s'éloigne de la surface, c'est qu'il n'y aura jamais intersection
			if (nextD > d) {
				return false;
			}
			d = nextD;
		}
		p_hitRecord._point	= position;
		p_hitRecord._normal = _evaluateNormal(position); 
		p_hitRecord.faceNormal( p_ray.getDirection() );
		p_hitRecord._distance = distance;
		p_hitRecord._object	  = this;
		return true;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		/// SphereTracing  
		float distance = p_tMin;
		Vec3f position = p_ray.getOrigin() + distance * p_ray.getDirection();
		float d = _sdf(position);
		// Tant que l'on est pas suffisament proche de la surface
		while (d > _minDistance) { 
			distance += d;
			if (distance > p_tMax) 
				return false;
			position = p_ray.getOrigin() + distance * p_ray.getDirection();
			const float nextD = _sdf(position);
			// Lorsque l'on s'éloigne de la surface, c'est qu'il n'y aura jamais intersection
			if (nextD > d) 
				return false;
			d = nextD;
		}
		return true;
	}
} // namespace RT_ISICG
