#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, float p_slope ) : _kd( p_kd ), _slope(p_slope) {
			/*
				kd: coefficient de réflectance diffus
				slope: Pente des microfacette
			*/
			float slopeSquared = _slope * _slope;
			A = 1.f - 0.5f * slopeSquared / (slopeSquared + 0.33f);
			B = 0.45f * slopeSquared / (slopeSquared + 0.09f);
		}  

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc) const { 
			
			const float cosThetaObs = glm::dot(p_normal, p_obs);
			const float cosThetaInc = glm::dot(p_normal, p_inc);

            const float thetaObs = glm::acos( cosThetaObs );
            const float thetaInc = glm::acos( cosThetaInc );
			
            const Vec3f phiObs = glm::normalize( p_obs - p_normal * cosThetaObs );
            const Vec3f phiInc = glm::normalize( p_inc - p_normal * cosThetaInc );
			const float gamma = std::max(0.f, glm::dot(phiObs, phiInc));
			  
			return (_kd * INV_PIf) 
				* A + (B * gamma
					* sin(std::max(thetaInc, thetaObs)) 
					* tan(std::min(thetaInc, thetaObs))
				); 
		} 

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _slope = 0.f;
		float A = 0.f;
		float B = 0.f;

	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_OREN_NAYAR__
