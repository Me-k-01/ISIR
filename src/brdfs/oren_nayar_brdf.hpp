#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_roughness ) : _kd( p_kd ), _roughness(p_roughness) {
			/*
				kd : coefficient de réflectance diffus
				slope : Pente des microfacette
			*/
			const float slopeSquared = p_roughness * p_roughness;
			_ambiant = _kd * INV_PIf;
			_A = 1.f - 0.5f * slopeSquared / (slopeSquared + 0.33f);
			_B = 0.45f * slopeSquared / (slopeSquared + 0.09f);
		}  
		 
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc) const { 
			
			const float cosThetaObs = glm::dot(p_normal, p_obs);
			const float cosThetaInc = glm::dot(p_normal, p_inc);
			  
			return evaluate(p_normal, p_obs, p_inc, cosThetaObs, cosThetaInc); 
		} 

		// Cas ou le cos théta est précalculé
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc, const float p_cosThetaObs, const float p_cosThetaInc) const {
            const float thetaObs = glm::acos( p_cosThetaObs );
            const float thetaInc = glm::acos( p_cosThetaInc );

			float maxi = thetaInc;	// alpha
			float mini = thetaObs; // beta
			if (thetaInc < thetaObs) {
				maxi = thetaObs;
				mini = thetaInc;
			}
			
            const Vec3f woObs = glm::normalize( p_obs - p_normal * p_cosThetaObs );
            const Vec3f wiPhi = glm::normalize( p_inc - p_normal * p_cosThetaInc );

			const float gamma = std::max(0.f, glm::dot(woObs, wiPhi));
			  
			return _ambiant * (_A + (_B * gamma * sin(maxi) * tan(mini))); 
		} 


		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		Vec3f _ambiant = WHITE;
		float _roughness = 0.f;
		float _A = 0.f;
		float _B = 0.f;

	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_OREN_NAYAR__
