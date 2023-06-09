#include "direct_lighting_integrator.hpp"

#define CASTED_SHADOW

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( ! p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
			return _backgroundColor; 

		// Ajouter la contribution des sources lumineuses de la scene 		
		return _directLighting(p_scene, p_ray, hitRecord); 
	}

	Vec3f DirectLightingIntegrator::_directLighting(const Scene & p_scene, const Ray & p_ray, const HitRecord & p_hitRecord) const {
			Vec3f li = Vec3f(0.f);
			BaseMaterial * mat = p_hitRecord._object->getMaterial();
			//Vec3f mtlColor = mat->getFlatColor(); 	

			for (const auto & light : p_scene.getLights()) {
				// Dans le cas d'une lumi�re surfacique, on tire plusieurs rayons pour echantillonner de mani�re al�atoire la surface.
				const int nbLightRay = light->getIsSurface() ? _nbLightSamples : 1;

				Vec3f col = Vec3f(0.f);
				for (int i = 0; i < nbLightRay; i++) {
					const LightSample lightSample = light->sample(p_hitRecord._point);
					// Determiner si le point observ� est �clair� ou non (pour les ombres port�es)
					Ray shadowRay = Ray(p_hitRecord._point, lightSample._direction);
					shadowRay.offset(p_hitRecord._normal);
					 
#ifdef CASTED_SHADOW
					// Ombrage
					// On lance un rayon en direction de la lumi�re, et s'il est obstru� on skip � cause de l'ombrage
					if (p_scene.intersectAny(shadowRay, 0.f, lightSample._distance))  
						continue;	
#endif // CASTED_SHADOW

					const float cosTheta = glm::dot(p_hitRecord._normal, lightSample._direction); 
					// Eclairage de lambert
					//col += mtlColor * lightSample._radiance * std::max(cosTheta, 0.f);
					col += mat->shade(p_ray, p_hitRecord, lightSample) * lightSample._radiance * std::max(cosTheta, 0.f); 
				}
				li += col / (float)nbLightRay;
				continue;
			}
			return li;
	}
} // namespace RT_ISICG
