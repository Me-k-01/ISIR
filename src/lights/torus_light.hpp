#ifndef __RT_ISICG_TORUS_LIGHT__
#define __RT_ISICG_TORUS_LIGHT__

#include "base_light.hpp" 

namespace RT_ISICG
{
	class TorusLight : public BaseLight
	{
		public:

			TorusLight( const Vec3f & p_center, const float p_radius, const float p_thickness, const Vec3f & p_color, const float p_power ) : 
				BaseLight(p_color, p_power), 
				_center(p_center), 
				_radius(p_radius),
				_thickness(p_thickness),
				_area(TWO_PIf * _radius * TWO_PIf * _thickness) 
			{
				_isSurface = true;
			}
			virtual LightSample sample( const Vec3f & p_point ) const override;
		private:
			void _rdmPoint(Vec3f & p_point, Vec3f & p_normal) const; // renvoie un point random sur la surface
		protected:

			Vec3f _center = Vec3f(0.f); 
			// Avec  _thickness <= _radius
			const float _radius; // distance entre centre du torus et le centre du tube
			const float _thickness; // distance entre le centre du tube et la surface du torus
			const float _area; 
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_TORUS_LIGHT__