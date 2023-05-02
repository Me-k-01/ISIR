#include "path_tracing_integrator.hpp"

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
											const Ray& p_ray,
											const HitRecord& p_hitRecord,
											const float p_tMin,
											const float p_tMax,
											const int p_iBounces,
											const bool p_isInAir) const
	{
		return Vec3f(0);
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
		return _directLighting(p_scene, p_ray, hitRecord) + _indirectLighting(p_scene, p_ray, hitRecord, p_tMin, p_tMax, p_iBounces, p_isInAir); ; 
	}
} // namespace RT_ISICG
