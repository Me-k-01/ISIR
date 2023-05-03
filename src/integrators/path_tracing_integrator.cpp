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


		// On echantillone des rayons pour calculer l'éclairage indirect.
		Vec3f indirectColor = Vec3f(0.f);
		for (int i = 0; i < _nbIndirectSample; i++) {
			const Vec3f sample = _sampleUniformHemisphere(p_hitRecord._normal);  
			Ray indirectSampleRay(Vec3f(p_hitRecord._point), sample);
			// Ne pas retaper dans la surface
			indirectSampleRay.offset(p_hitRecord._normal);
			 
			// L'intensité de la couleur indirect dépends de l'angle sur lequelle on a tirer le rayon
			// Rq : Pour accélerer la convergence vers l'intégral plus rapidement, il faudrait privilégier de tirer des rayons avec un faible angle par rapport a la normal, plutot que d'avoir une distribution uniforme.
			const float cosTheta = glm::dot(p_hitRecord._normal, indirectSampleRay.getDirection()); 
			
			#ifdef COSINE_WEIGHTED_HEMISPHERE
				const float pdf = 1.f / TWO_PIf; 
			#else		
				const float pdf = cosTheta / TWO_PIf; // On pourra retirer cosTheta
			#endif // COSINE_WEIGHTED_HEMISPHERE 

			indirectColor += _trace(p_scene, indirectSampleRay, p_tMin, p_tMax, p_iBounces + 1, p_isInAir) *  cosTheta / pdf; 
		}   

		// (Somme / pdf) / indirectLightSample, avec pdf = 1 / (2 * PI)
		// marche
		//return (indirectColor) / (float)_nbIndirectSample;
		// marche pas 
		//return (TWO_PIf / (float)_nbIndirectSample) * indirectColor;
		//   
		const float cosTheta = glm::dot(p_hitRecord._normal, -p_ray.getDirection());
		//return (indirectColor / ((float)_nbIndirectSample * pdf) ) * std::max(0.f, cosTheta);
		return (indirectColor / ((float)_nbIndirectSample) ) * std::max(0.f, cosTheta);
		 

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
} // namespace RT_ISICG
