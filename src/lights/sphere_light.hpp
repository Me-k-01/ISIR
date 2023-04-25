#ifndef __RT_ISICG_SPHERE_LIGHT__
#define __RT_ISICG_SPHERE_LIGHT__

#include "base_light.hpp" 

namespace RT_ISICG
{
	class SphereLight : public BaseLight
	{
		public:

			SphereLight( const Vec3f & p_center, const float p_radius, const Vec3f & p_color, const float p_power ) : 
				BaseLight(p_color, p_power), 
				_center(p_center), 
				_radius(p_radius),
				_area(4.f * PIf * p_radius * 2.f ) 
			{
				_isSurface = true;
			}
			virtual LightSample sample( const Vec3f & p_point ) const override;
		private:
			Vec3f _rdmPoint() const; // renvoie un point random sur la surface
		protected:

			Vec3f _center = Vec3f(0.f); 
			const float _radius;
			const float _area; 
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE_LIGHT__