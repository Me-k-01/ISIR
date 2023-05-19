#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

//#define USE_BLINN_PHONG

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_kd, const Vec3f & p_ks, const float p_s) : _kd(p_kd), _ks(p_ks), _shininess(p_s)  {
			/*
				kd : coefficient de réflectance diffus 
				ks : coefficient de réflectance spéculaire
				_s : brillance du materiaux
			*/  
			_ambiant = p_kd * INV_PIf;
		}  

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc) const { 
			

			const float cosThetaInc = glm::dot(p_normal, p_inc); 
#ifdef USE_BLINN_PHONG
				const Vec3f h = glm::normalize(p_obs + p_inc); // demi vecteur entre Lo et Li, norm(Lo - Li) 
				const float cosAlpha = glm::dot(p_normal, h);
#else
				const Vec3f reflect = glm::reflect(-p_inc, p_normal);
				// Alpha : angle entre la direction d'observation et la normal
				const float cosAlpha = glm::dot(p_obs, reflect); 
#endif // USE_BLIN_PHONG
			  

			return _ambiant + (_ks / std::max(0.f, cosThetaInc)) * pow(std::max(0.f, cosAlpha), _shininess);
		} 

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _ambiant = WHITE;
		Vec3f _ks;
		Vec3f _kd;
		float _shininess;

	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
