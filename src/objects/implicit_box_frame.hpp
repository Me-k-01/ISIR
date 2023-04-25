#ifndef __RT_ISICG_IMPLICIT_BOX_FRAME_SURFACE__
#define __RT_ISICG_IMPLICIT_BOX_FRAME_SURFACE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitBoxFrame : public ImplicitSurface
	{
	  public:
		  ImplicitBoxFrame(const std::string & p_name, const Vec3f & p_center, const Vec3f & p_size, const float p_e) : 
			  ImplicitSurface(p_name), _center(p_center), _halfSize(p_size/2.f), _e(p_e)
		  {}
		virtual ~ImplicitBoxFrame() = default;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{  			
			const Vec3f p = abs(p_point - _center) - _halfSize;
			const Vec3f q = abs(p + _e) - _e;
			return glm::min(glm::min(
				glm::length(glm::max(Vec3f(p.x,q.y,q.z), 0.f)) + glm::min(glm::max(p.x,glm::max(q.y,q.z)), 0.f),
				glm::length(glm::max(Vec3f(q.x,p.y,q.z), 0.f)) + glm::min(glm::max(q.x,glm::max(p.y,q.z)), 0.f)),
				glm::length(glm::max(Vec3f(q.x,q.y,p.z), 0.f)) + glm::min(glm::max(q.x,glm::max(q.y,p.z)), 0.f));
		}
	  private:
		const Vec3f _center; 
		const Vec3f _halfSize;
		const float _e;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BOX_FRAME_SURFACE__
