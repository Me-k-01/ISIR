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
	Vec3f WhittedIntegrator::_reflectTrace(const Scene& p_scene,
		const Ray& p_ray,
		const HitRecord& p_hitRecord,
		const float p_tMin,
		const float p_tMax,
		const int p_iBounces,
		const bool p_isInAir) const
	{ 
		const Vec3f reflect = glm::reflect(p_ray.getDirection(), p_hitRecord._normal);
		Ray reflectedRay(Vec3f(p_hitRecord._point), glm::normalize(reflect));
		// Pour eviter de retapper sur la même surface
		reflectedRay.offset(p_hitRecord._normal);
		return _trace(p_scene, reflectedRay, p_tMin, p_tMax, p_iBounces + 1, p_isInAir);
	}
	// Calcul la coloration d'un materiau transparent
	Vec3f WhittedIntegrator::_refractTrace( const Scene & p_scene, 
										    const Ray & p_ray, 
										    const HitRecord & p_hitRecord, 
											const float p_tMin, 
											const float p_tMax,
											const int p_iBounces, 
											const bool p_isInAir) const  
	{ 
		 
		BaseMaterial * mat = p_hitRecord._object->getMaterial();
		const float ior = mat->getIOR(); // indice de refraction  			
		const Vec3f inRay = p_ray.getDirection();

		// Calculer la reflection 
		const Vec3f reflect = glm::reflect(inRay, p_hitRecord._normal);
		Ray reflectedRay(Vec3f(p_hitRecord._point), glm::normalize(reflect));
		reflectedRay.offset(p_hitRecord._normal);
		const Vec3f reflection = _trace(p_scene, reflectedRay, p_tMin, p_tMax, p_iBounces + 1, p_isInAir);

		// Si on entre ou sort de l'objet 
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
		const float cosThetaI = glm::dot(-inRay, p_hitRecord._normal);
		// avec Snell law
		const float sinThetaI = sin(acos(cosThetaI));
		if (sinThetaI > 1.f) 
			return reflection;
			
		const float cosThetaT = sqrt(1.f - pow(eta * sinThetaI, 2));

		// équation de Fresnel
		const float rs = pow((n1 * cosThetaI - n2 * cosThetaT) / 
								(n1 * cosThetaI + n2 * cosThetaT), 2);

		const float rp = pow((n1 * cosThetaT - n2 * cosThetaI) / 
								(n1 * cosThetaT + n2 * cosThetaI), 2);

		const float kr = .5f * (rs + rp);// proportion de lumière réfléchie 
			
		if (kr == 1.f) // cas de la reflection total 
			return reflection;

		// Calculer la refraction
		const Vec3f refract = glm::refract(inRay, p_hitRecord._normal, eta);
		Ray refractedRay(Vec3f(p_hitRecord._point), glm::normalize(refract));
		refractedRay.offset(-p_hitRecord._normal); // Pour ne pas retaper sur la surface
		const Vec3f refraction = _trace(p_scene, refractedRay, p_tMin, p_tMax, p_iBounces + 1, ! p_isInAir);
			
		return reflection * kr + refraction * (1.f - kr);
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
		
		hitRecord.faceNormal(p_ray.getDirection());

		if (mat->isMirror()) { 
			// Calculer la reflection 
			return _reflectTrace(p_scene, p_ray, hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir);
		}
		if (mat->isTransparent()) {
			// Calculer la reflection et refraction
			return _refractTrace(p_scene, p_ray, hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir);
		}
		return _directLighting(p_scene, p_ray, hitRecord);
	}
} // namespace RT_ISICG
