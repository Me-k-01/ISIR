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
		return _directLighting(p_scene, hitRecord); 
		
	}

	Vec3f DirectLightingIntegrator::_directLighting(const Scene & p_scene, const HitRecord & p_hitRecord) const {
			Vec3f li = Vec3f(0.f);
			BaseMaterial * mat = p_hitRecord._object->getMaterial();
			//Vec3f mtlColor = mat->getFlatColor(); 	
			
			

			for (const auto & light : p_scene.getLights()) {

				// Cas d'une lumière surfacique
				if (light->getIsSurface()) {
					Vec3f col = Vec3f(0);
					for (int i = 0; i < _nbLightSamples; i++) {
						const LightSample lightSample = light->sample(p_hitRecord._point);
						// Determiner si le point observé est éclairé ou non (pour les ombres portés)
						Ray shadowRay = Ray(p_hitRecord._point, lightSample._direction);
						shadowRay.offset(p_hitRecord._normal);
						if (p_scene.intersectAny(shadowRay, 0, lightSample._distance)) { // On lance un rayon en direction de la lumière, et s'il est obstrué on skip à cause de l'ombrage
							continue;
						} 

						float cosTheta = glm::dot(p_hitRecord._normal, lightSample._direction); 
						//col += mtlColor * lightSample._radiance * std::max(cosTheta, 0.f);
						// Eclairage de lambert
						col += mat->shade(shadowRay, p_hitRecord, lightSample) * lightSample._radiance * std::max(cosTheta, 0.f); 
					}
					li += col / (float)_nbLightSamples;
					continue;
				} 

				const LightSample lightSample = light->sample(p_hitRecord._point);
				// Determiner si le point observé est éclairé ou non (pour les ombres portés)
				Ray shadowRay = Ray(p_hitRecord._point, lightSample._direction);
				shadowRay.offset(p_hitRecord._normal);
				if (p_scene.intersectAny(shadowRay, 0, lightSample._distance)) { // On lance un rayon en direction de la lumière, et s'il est obstrué on skip à cause de l'ombrage
					continue;
				} 

				float cosTheta = glm::dot(p_hitRecord._normal, lightSample._direction); 
				//li += mtlColor * lightSample._radiance * std::max(cosTheta, 0.f);
				// Eclairage de lambert
				li += mat->shade(shadowRay, p_hitRecord, lightSample) * lightSample._radiance * std::max(cosTheta, 0.f); 
			}
			return li;
	}
} // namespace RT_ISICG
