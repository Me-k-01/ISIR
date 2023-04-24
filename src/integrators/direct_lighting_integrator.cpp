#include "direct_lighting_integrator.hpp"

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

				// Dans le cas d'une lumière surfacique, on tire plusieurs rayons pour echantillonner de manière aléatoire la surface.
				const float nbLightRay = light->getIsSurface() ? (float)_nbLightSamples : 1.f;

				Vec3f col = Vec3f(0.f);
				for (int i = 0; i < nbLightRay; i++) {
					const LightSample lightSample = light->sample(p_hitRecord._point);
					// Determiner si le point observé est éclairé ou non (pour les ombres portées)
					Ray shadowRay = Ray(p_hitRecord._point, lightSample._direction);
					shadowRay.offset(p_hitRecord._normal);
					if (p_scene.intersectAny(shadowRay, 0, lightSample._distance)) { // On lance un rayon en direction de la lumière, et s'il est obstrué on skip à cause de l'ombrage
						continue;
					} 

					const float cosTheta = glm::dot(p_hitRecord._normal, lightSample._direction); 
					//col += mtlColor * lightSample._radiance * std::max(cosTheta, 0.f);
					// Eclairage de lambert
					col += mat->shade(p_ray, p_hitRecord, lightSample) * lightSample._radiance * std::max(cosTheta, 0.f); 
				}
				li += col / nbLightRay;
				continue;
			}
			return li;
	}
} // namespace RT_ISICG
