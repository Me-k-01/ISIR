#ifndef __RT_ISICG_IMPLICIT_SPHERE_SURFACE__
#define __RT_ISICG_IMPLICIT_SPHERE_SURFACE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitSphere : public ImplicitSurface
	{
	  public:
		  ImplicitSphere(const std::string & p_name, const Vec3f & p_center, const float p_radius) : ImplicitSurface(p_name), _center(p_center), _radius(p_radius) {}
		virtual ~ImplicitSphere() = default;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{
			return glm::length(p_point - _center) - _radius;
		}
	  private:
		const float _radius;
		const Vec3f _center;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE_SURFACE__
