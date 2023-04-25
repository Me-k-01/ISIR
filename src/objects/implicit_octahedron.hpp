#ifndef __RT_ISICG_IMPLICIT_OCTAHEDRON_SURFACE__
#define __RT_ISICG_IMPLICIT_OCTAHEDRON_SURFACE__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitOctahedron : public ImplicitSurface
	{
	  public:
		  ImplicitOctahedron(const std::string & p_name, const Vec3f & p_center, const float p_size) : 
			  ImplicitSurface(p_name), _center(p_center), _size(p_size)
		  {}
		virtual ~ImplicitOctahedron() = default;

	  private:
		// Signed Distance Function
		virtual float _sdf(const Vec3f& p_point) const override
		{  
			Vec3f p = abs(p_point - _center); 
			const float m = p.x + p.y + p.z - _size;
			Vec3f q;
			if (3.f * p.x < m)
				q = Vec3f(p);
			else if( 3.f * p.y < m ) 
				q = Vec3f(p.y, p.z, p.x);
			else if( 3.f * p.z < m ) 
				q = Vec3f(p.z, p.x, p.y);
			else 
				return m * 0.57735027f;
    
			float k = glm::clamp(.5f * (q.z - q.y + _size), 0.f, _size); 

			return glm::length(Vec3f(q.x, q.y - _size + k, q.z - k)); 
			
		}
	  private:
		const Vec3f _center; 
		const float _size;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_OCTAHEDRON_SURFACE__
