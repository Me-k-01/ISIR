#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"
#include "brdfs/oren_nayar_brdf.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		  CookTorranceBRDF(const Vec3f & p_kd, const float p_metalness, const float p_roughness) : _kd(p_kd), _roughness(p_roughness) {
			/*
				kd : coefficient de réflectance diffus
				roughness : Pente des microfacette
			*/ 
			_ambiant = p_kd * INV_PIf;
			_rSqrdSqrd = pow(p_roughness, 4.f);
			_metalness = p_metalness;
			_orenNayarBRDF = new OrenNayarBRDF(p_kd, p_roughness);
			_smithK = pow((p_roughness + 1.f), 2.f) / 8.f;
		}  

		 
		inline Vec3f evaluate(const Vec3f & p_normal, const Vec3f & p_obs, const Vec3f & p_inc) const { 
			// Precalcul du cos entre l'angle de la normal et des rayons pour les deux equation
			const float cosThetaObs = glm::dot(p_normal, p_obs);
			const float cosThetaInc = glm::dot(p_normal, p_inc);

			//////////// Partie diffuse ////////////
			const Vec3f diffuse = _orenNayarBRDF->evaluate(p_normal, p_obs, p_inc, cosThetaObs, cosThetaInc);
			  
			//////////// Partie speculaire ////////////
			const Vec3f h = glm::normalize(p_obs + p_inc); // demi vecteur entre Lo et Li, norm(Lo - Li) 

			const float cosNH = glm::dot(p_normal, h);
			const float d = _normalDistrib(p_normal, h);
			const float g = _mask(cosThetaObs, cosThetaInc);
			const Vec3f f = _reflectionAmount(p_obs, h);
			const Vec3f specular = (d * f * g) / (4.f * cosThetaObs * cosThetaInc);

			return (1.f - _metalness) * diffuse  + _metalness * specular; 
		} 

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE; // F0 : reflectance dans la direction de la normal
		Vec3f _ambiant = WHITE; // _kd / PI
		float _metalness = 0.f;
		float _roughness = 0.f;
		float _rSqrdSqrd = 0.f; // Roughness squared squared = alpha² 

		OrenNayarBRDF * _orenNayarBRDF = nullptr;
		// Précalcul du k pour la fonction de masquage
		float _smithK = 0.f;
		
		// D : Distribution des normals de Trowbridge-Reitz
		inline float _normalDistrib(const Vec3f & p_normal, const Vec3f & p_h) const {
            const float cosNH = glm::dot(p_normal, p_h);
			 
			const float den = (cosNH * cosNH) * (_rSqrdSqrd - 1.f) + 1.f;
			return _rSqrdSqrd / (PIf * den * den);
		}
		
		// G : Fonction de masquage des faces cachées parmi les micro-facettes avec le model de Smith
		inline float _mask(const float p_cosThetaObs, const float p_cosThetaInc) const {
			return _g1(p_cosThetaObs) * _g1(p_cosThetaInc);
		}
		inline float _g1(const float p_x) const {
			return p_x / (p_x * (1.f - _smithK) + _smithK);
		}

		// F : Fonction qui décrit la proportion de réflection de la surface d'un angle donné. Par une approximation de Schlick de l'équation de Fresnel,
		inline Vec3f _reflectionAmount(const Vec3f & p_obs, const Vec3f & p_h) const {
			return _kd + (1.f - _kd) * (1.f - glm::dot(p_h, p_obs));
		}
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__
