#include "path_tracing_integrator.hpp"
#include "utils/random.hpp" 

#define COSINE_WEIGHTED_HEMISPHERE

namespace RT_ISICG
{
	Vec3f PathTracingIntegrator::Li( const Scene & p_scene,
									 const Ray &   p_ray,
									 const float   p_tMin,
									 const float   p_tMax) const
	{
		// origine du rayon primaire par du vide
		return _trace(p_scene, p_ray, p_tMin, p_tMax, 0, true); // Lance la recurtion
	}
	Vec3f PathTracingIntegrator::_indirectLighting( const Scene& p_scene,
											const Ray &   p_ray,
											const HitRecord& p_hitRecord,
											const float p_tMin,
											const float p_tMax,
											const int p_iBounces,
											const bool p_isInAir) const
	{
		// Tant que le nombre de rebond est plus petit au rebond max
		if (p_iBounces > _nbBounces) 
			return BLACK;
		BaseMaterial * mat = p_hitRecord._object->getMaterial();

		// On echantillone des rayons pour calculer l'éclairage indirect.
		Vec3f indirectColor = Vec3f(0.f);
		for (int i = 0; i < _nbIndirectSample; i++) {
			#ifdef COSINE_WEIGHTED_HEMISPHERE
				const Vec3f sampleDir = _sampleCosineWeigthedHemisphere(p_hitRecord._normal);
			#else		
				const Vec3f sampleDir = _sampleUniformHemisphere(p_hitRecord._normal);
			#endif // COSINE_WEIGHTED_HEMISPHERE   
			Ray indirectSampleRay(Vec3f(p_hitRecord._point), sampleDir);
			// Ne pas retaper dans la surface
			indirectSampleRay.offset(p_hitRecord._normal);

			HitRecord hitRecord; 
			Vec3f li = _trace(p_scene, indirectSampleRay, hitRecord, p_tMin, p_tMax, p_iBounces + 1, p_isInAir);
			const float dist = hitRecord._distance;

			if (li != _backgroundColor) { // Eclairé par autre chose que le background
				LightSample indirectLight(sampleDir, dist, li / (dist * dist), 1.f);
				const Vec3f fr = mat->shade(indirectSampleRay, p_hitRecord, indirectLight);
				// Li * fr * cosTheta / pdf
				#ifdef COSINE_WEIGHTED_HEMISPHERE
					// Pour accélerer le calcul de l'intégral de Monte Carlo, on privilégie tirer des rayons qui ont le plus gros impacte.
					// Notre distribution de normal depends du cosinus de leurs angle avec la normal.
					// On retire le cos theta qui s'annule à cause de la pdf (distibution des normales)
					indirectColor += fr * li;
				#else
					// L'intensité de la couleur indirect dépends de l'angle sur lequelle on a tirer le rayon 
					const float cosTheta = glm::dot(p_hitRecord._normal, sampleDir); 
					indirectColor += fr * li * cosTheta;
				#endif // COSINE_WEIGHTED_HEMISPHERE
			}
		}
		#ifdef COSINE_WEIGHTED_HEMISPHERE
			// Nous avons déjà annulé le cos theta précédamment, donc pas besoin de le rajouter ici 
			const float pdf = 1.f / PIf; 
		#else		 
			const float pdf = 1.f / TWO_PIf; 
		#endif // COSINE_WEIGHTED_HEMISPHERE 
			 
		return (indirectColor / pdf / ((float)_nbIndirectSample) );
	}

	Vec3f PathTracingIntegrator::_trace( const Scene & p_scene,
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
		return _directLighting(p_scene, p_ray, hitRecord) + _indirectLighting(p_scene, p_ray, hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir)  ;
	}

	Vec3f PathTracingIntegrator::_trace( const Scene & p_scene,
										 const Ray &   p_ray,
										 HitRecord &   p_hitRecord, // On a besoin de ressortir le hit record pour avoir la distance de l'éclairage indirecte
										 const float   p_tMin,
										 const float   p_tMax,
										 const int     p_iBounces,
										 const bool    p_isInAir ) const 
	{
		if (p_iBounces > _nbBounces) // Si trop de rebond a cause de mirroire face à face
			return BLACK;
		 
		if ( ! p_scene.intersect( p_ray, p_tMin, p_tMax, p_hitRecord ) )
			return _backgroundColor; 

		// Ajouter la contribution des sources lumineuses de la scene 	
		BaseMaterial * mat = p_hitRecord._object->getMaterial();
		
		p_hitRecord.faceNormal(p_ray.getDirection());

		if (mat->isMirror()) { 
			// Calculer la reflection 
			return _reflectTrace(p_scene, p_ray, p_hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir);
		}
		if (mat->isTransparent()) {
			// Calculer la reflection et refraction
			return _refractTrace(p_scene, p_ray, p_hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir);
		} 
		return _directLighting(p_scene, p_ray, p_hitRecord) + _indirectLighting(p_scene, p_ray, p_hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir)  ;
	}
} // namespace RT_ISICG
