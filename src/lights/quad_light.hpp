#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp" 

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
		public:

			QuadLight( const Vec3f & p_position, const Vec3f & p_u, const Vec3f & p_v, const Vec3f & p_color, const float p_power ) : BaseLight(p_color, p_power), 
				_position(p_position), _u(p_u), _v(p_v), _n(glm::normalize(glm::cross(p_u, p_v))), _area(glm::length(p_u) * glm::length(p_v)) {}
			virtual LightSample sample( const Vec3f & p_point ) const override;
		protected:

			Vec3f _position = Vec3f(0.f); // Coin du quad
			Vec3f _u; Vec3f _v; // Deux arrêtes partants du point
			Vec3f _n = Vec3f(1.f); // Normal
			float _area = 0.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_QUAD_LIGHT__