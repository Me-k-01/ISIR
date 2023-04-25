#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax) const
	{
		// origine du rayon primaire par du vide
		return _trace(p_scene, p_ray, p_tMin, p_tMax, 0, true); // Lance la recurtion
	}

	Vec3f WhittedIntegrator::_trace( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax,
								 const int p_iBounces,
								 const bool p_isInAir ) const
	{
		if (p_iBounces > _nbBounces) // Si trop de rebond a cause de mirroire face à face
			return BLACK;
		
		HitRecord hitRecord;
		if ( ! p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
			return _backgroundColor; 

		// Ajouter la contribution des sources lumineuses de la scene 	
		BaseMaterial * mat = hitRecord._object->getMaterial();
		if (mat->isMirror()) { 
			// Calculer la reflection
			hitRecord.faceNormal(p_ray.getDirection());
			const Vec3f reflect = glm::reflect(p_ray.getDirection(), hitRecord._normal);
			Ray reflectedRay(Vec3f(hitRecord._point), glm::normalize(reflect));
			reflectedRay.offset(hitRecord._normal);
			return _trace(p_scene, reflectedRay, p_tMin, p_tMax, p_iBounces + 1, p_isInAir);
		}
		if (mat->isTransparent()) {
			// Si on entre ou sort de l'objet 

			//if (p_ior == )
			const float ior = mat->getIOR(); // indice de refraction  			
			const Vec3f inRay = p_ray.getDirection();

			// Calculer la reflection
			hitRecord.faceNormal(inRay);
			const Vec3f reflect = glm::reflect(inRay, hitRecord._normal);
			Ray reflectedRay(Vec3f(hitRecord._point), glm::normalize(reflect));
			reflectedRay.offset(hitRecord._normal);
			const Vec3f reflection = _trace(p_scene, reflectedRay, p_tMin, p_tMax, p_iBounces + 1, p_isInAir);
			float n1, n2;
			if (p_isInAir) {
				n1 = 1.f;
				n2 = ior;
			}
			else {
				n1 = ior;
				n2 = 1.f;
			}
			const float eta = n1 / n2;
			// Cos teta transmis 
			const float cosThetaI = glm::dot(-inRay, hitRecord._normal);
			// avec Snell law
			const float sinThetaI = sin(acos(cosThetaI));
			if (sinThetaI > 1.f) {
				return reflection;
			}
			const float cosThetaT = sqrt(1.f - pow(eta * sinThetaI, 2));//glm::dot(refract, - hitRecord._normal);

			// équation de Fresnel
			const float rs = pow((n1 * cosThetaI - n2 * cosThetaT) / 
								 (n1 * cosThetaI + n2 * cosThetaT), 2);

			const float rp = pow((n1 * cosThetaT - n2 * cosThetaI) / 
								 (n1 * cosThetaT + n2 * cosThetaI), 2);

			const float kr = .5f * (rs + rp);// proportion de lumière réfléchie 
			
			if (kr == 1.f) // cas de la reflection total 
				return reflection;

			// Calculer la refraction
			const Vec3f refract = glm::refract(inRay, hitRecord._normal, eta);
			Ray refractedRay(Vec3f(hitRecord._point), glm::normalize(refract));
			refractedRay.offset(-hitRecord._normal); // Pour ne pas retaper sur la surface
			const Vec3f refraction = _trace(p_scene, refractedRay, p_tMin, p_tMax, p_iBounces + 1, ! p_isInAir);
			
			return reflection * kr + refraction * (1.f - kr);
		}
		return _directLighting(p_scene, p_ray, hitRecord); 
		
	}

	Vec3f WhittedIntegrator::_directLighting(const Scene & p_scene, const Ray & p_ray, const HitRecord & p_hitRecord) const {
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
					col += mat->shade(p_ray, p_hitRecord, lightSample) * lightSample._radiance * std::max(cosTheta, 0.f); 
				}
				li += col / nbLightRay;	
				continue;
			}
			return li;
	}
} // namespace RT_ISICG
