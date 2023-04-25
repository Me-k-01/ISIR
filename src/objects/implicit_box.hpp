#ifndef __RT_ISICG_IMPLICIT_BOX_SURFACE__
#define __RT_ISICG_IMPLICIT_BOX_SURFACE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitBox : public ImplicitSurface
	{
	  public:
		  ImplicitBox(const std::string & p_name, const Vec3f & p_center, const Vec3f & p_size) : 
			  ImplicitSurface(p_name), _center(p_center), _halfSize(p_size/2.f)
		  {}
		virtual ~ImplicitBox() = default;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{ 
			Vec3f q = abs(p_point - _center) - _halfSize;
			return glm::length(glm::max(q, 0.f)) + glm::min(
				glm::max(q.x, glm::max(q.y,q.z)),
			0.f);
		}
	  private:
		const Vec3f _center; 
		const Vec3f _halfSize;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BOX_SURFACE__
