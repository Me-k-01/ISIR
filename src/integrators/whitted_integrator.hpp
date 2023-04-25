#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public DirectLightingIntegrator
	{
		public:
			WhittedIntegrator(const int p_nbLightSamples) : DirectLightingIntegrator(p_nbLightSamples) {}
			WhittedIntegrator(const int p_nbLightSamples, const int p_nbBounces) : DirectLightingIntegrator(p_nbLightSamples), _nbBounces(p_nbBounces) {}
			virtual ~WhittedIntegrator() = default;

			const IntegratorType getType() const override { return IntegratorType::WHITTED; }
			// Return incoming luminance.
			Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax) const override;

		private :
			Vec3f _trace( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
			int _nbBounces = 5; // nombres de rebonds maximaux 
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
