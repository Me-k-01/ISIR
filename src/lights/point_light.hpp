#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp" 

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
		public:

			PointLight( const Vec3f p_position, const Vec3f & p_color, const float p_power ) : BaseLight(p_color, p_power), _position(p_position) {}
			virtual LightSample sample( const Vec3f & p_point ) const override;
		protected:

			Vec3f _position = Vec3f(0.f);
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__