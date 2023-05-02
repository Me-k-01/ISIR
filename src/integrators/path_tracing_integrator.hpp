#ifndef __RT_ISICG_PATH_TRACING_INTEGRATOR__
#define __RT_ISICG_PATH_TRACING_INTEGRATOR__

#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	class PathTracingIntegrator : public WhittedIntegrator
	{
		public:
			PathTracingIntegrator(const int p_nbLightSamples) : WhittedIntegrator(p_nbLightSamples) {}
			PathTracingIntegrator(const int p_nbLightSamples, const int p_nbBounces) : WhittedIntegrator(p_nbLightSamples, p_nbBounces) {}
			virtual ~PathTracingIntegrator() = default;

			const IntegratorType getType() const override { return IntegratorType::PATH_TRACED; }
			// Return incoming luminance.
			Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax) const override;

		protected :
			Vec3f _trace( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
			
			Vec3f _indirectLighting( const Scene & p_scene, const Ray & p_ray, const HitRecord & hitRecord, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;

			Vec3f _sampleHemisphere(const Vec3f p_normal) {
				return Vec3f(0);
			}
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PATH_TRACING_INTEGRATOR__
