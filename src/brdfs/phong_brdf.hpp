#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

#define USE_BLIN_PHONG

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_kd, const float p_ks, const float p_s) : _kd(p_kd), _coefSpec(p_ks), _shininess(p_s)  {
			/*
				kd : coefficient de r�flectance diffus 
				ks : coefficient de r�flectance sp�culaire
				_s : brillance du materiaux
			*/  
			_ambiant = p_kd * INV_PIf;
		}  

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc) const { 
			

			const float cosThetaInc = glm::dot(p_normal, p_inc);
			float cosAlpha;
			#ifdef USE_BLIN_PHONG
				const Vec3f h = glm::normalize(p_obs + p_inc); // demi vecteur entre Lo et Li, norm(Lo - Li) 
				cosAlpha = glm::dot(p_normal, h);
			#else
				const Vec3f reflect = glm::reflect(-p_inc, p_normal);
				// Alpha : angle entre la direction d'observation 
				cosAlpha = glm::dot(p_obs, reflect); 
			#endif // USE_BLIN_PHONG
			  

			return _ambiant + (_coefSpec / std::max(0.f, cosThetaInc)) * pow(std::max(0.f, cosAlpha), _shininess);
		} 

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _ambiant = WHITE;
		Vec3f _coefSpec;
		Vec3f _kd;
		float _shininess;

	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__