#ifndef __RT_ISICG_IMPLICIT_TORUS_SURFACE__
#define __RT_ISICG_IMPLICIT_TORUS_SURFACE__

#include "implicit_surface.hpp"
//#define VERTICAL

namespace RT_ISICG
{
	class ImplicitTorus : public ImplicitSurface
	{
	  public:
		  ImplicitTorus(const std::string & p_name, const Vec3f & p_center, const float p_radius, const float p_thickness) : ImplicitSurface(p_name), _center(p_center), _radius(p_radius), _thickness(p_thickness) {}
		virtual ~ImplicitTorus() = default;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{
			Vec3f point = p_point - _center;  
			// Donut vertical
#ifdef VERTICAL
			Vec2f q = Vec2f(glm::length(Vec2f(point.x, point.y)) - _radius, point.z);  
#else
			Vec2f q = Vec2f(glm::length(Vec2f(point.x, point.z)) - _radius, point.y);  
#endif // VERTICAL
			return length(q) - _thickness;
		}
	  private:
		const float _radius;
		const float _thickness;
		const Vec3f _center; 
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_TORUS_SURFACE__
