#ifndef __RT_ISICG_PATH_TRACING_INTEGRATOR__
#define __RT_ISICG_PATH_TRACING_INTEGRATOR__

#include "whitted_integrator.hpp"
#include "utils/random.hpp" 

namespace RT_ISICG
{
	class PathTracingIntegrator : public WhittedIntegrator
	{
		public:
			PathTracingIntegrator(const int p_nbLightSamples) : WhittedIntegrator(p_nbLightSamples) {}
			PathTracingIntegrator(const int p_nbLightSamples, const int p_nbIndirectSample) : WhittedIntegrator(p_nbLightSamples), _nbIndirectSample(p_nbIndirectSample) {}
			PathTracingIntegrator(const int p_nbLightSamples, const int p_nbIndirectSample, const int p_nbBounces) : WhittedIntegrator(p_nbLightSamples, p_nbBounces), _nbIndirectSample(p_nbIndirectSample) {}
			virtual ~PathTracingIntegrator() = default;

			const IntegratorType getType() const override { return IntegratorType::PATH_TRACED; }
			// Return incoming luminance.
			Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax) const override;

		protected :
			Vec3f _trace( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
			
			Vec3f _indirectLighting( const Scene & p_scene, const Ray & p_ray, const HitRecord & hitRecord, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
		private : 
			Vec3f _sampleUniformHemisphere( const Vec3f & p_normal ) const {
				const float theta = TWO_PIf * randomFloat(); 
				const float phi = acos(2.f * randomFloat() - 1.f); 
				
				// Hemisphere
				//const float theta = acos(randomFloat()); 
				//const float phi = TWO_PIf * randomFloat(); 

				Vec3f sample = Vec3f(
					cos(theta) * sin(phi), 
					cos(phi),
					sin(theta) * sin(phi)
				);
				// On change le rayon de sens pour ne prendre que sur l'hemisphere
				if (glm::dot(sample, p_normal) < 0.f) 
					sample = -sample;
				
				return sample;
			}
			Vec3f _sampleCosineWeigthedHemisphere(const Vec3f& p_normal) const {
				const float theta = acos(sqrt(randomFloat())); 
				const float phi = TWO_PIf * randomFloat(); 

				return Vec3f(
					cos(theta) * sin(phi), 
					cos(phi),
					sin(theta) * sin(phi)
				);
			}
			const int _nbIndirectSample = 3;

	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PATH_TRACING_INTEGRATOR__
