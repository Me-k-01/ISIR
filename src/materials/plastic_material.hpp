#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float p_ks, const float p_s )
			: BaseMaterial( p_name ), _brdf( p_diffuse, p_ks, p_s )
		{
		}

		virtual ~PlasticMaterial() = default;


		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
  
			// Direction observé: inverse de la caméra 
			return _brdf.evaluate(p_hitRecord._normal, glm::normalize( - p_ray.getDirection()), glm::normalize(p_lightSample._direction));
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		PhongBRDF _brdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PLASTIC_MATERIAL__
