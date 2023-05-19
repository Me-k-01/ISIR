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
			Vec3f _trace( const Scene & p_scene, const Ray & p_ray, HitRecord & p_hitRecord, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
			
			Vec3f _indirectLighting( const Scene & p_scene, const Ray & p_ray, const HitRecord & hitRecord, const float p_tMin, const float p_tMax, const int p_iBounces, const bool p_inAir) const;
		private : 
			Vec3f _rotateSample(const Vec3f & p_sample, const Vec3f & p_normal) const {
				// Création d'un systeme de coordonnée à partir de la normal (le vecteur up est la normal)
				Vec3f perpendicular, perpendicular2; // Nt et Nb
				// Cas ou les y sont plus grands que les x 
				
				perpendicular = std::fabs(p_normal.x) > std::fabs(p_normal.y) ?
					Vec3f(p_normal.z, 0, -p_normal.x) : 
					Vec3f(0, -p_normal.z, p_normal.y) ;

				perpendicular = glm::normalize( perpendicular );
				perpendicular2 = glm::normalize(glm::cross(p_normal, perpendicular));

				// Application de la matrice de transformation, du repère de la normal vers celui du monde.
				 return Vec3f(
					p_sample.x * perpendicular2.x + p_sample.y * p_normal.x + p_sample.z * perpendicular.x,
					p_sample.x * perpendicular2.y + p_sample.y * p_normal.y + p_sample.z * perpendicular.y,
					p_sample.x * perpendicular2.z + p_sample.y * p_normal.z + p_sample.z * perpendicular.z
				);
			}

			Vec3f _sampleUniformHemisphere( const Vec3f & p_normal ) const {
				// Point sur une sphere uniformement distribué
				const float theta = TWO_PIf * randomFloat(); 
				const float phi = acos(2.f * randomFloat() - 1.f); 
				
				Vec3f sample = glm::normalize(Vec3f(
					cos(theta) * sin(phi),
					cos(phi),
					sin(theta) * sin(phi)
				));
				// On flip le rayon pour echantilloner uniquement l'hemisphere orienté vers la normal.
				if (glm::dot(sample, p_normal) < 0.f) 
					sample = -sample;
				return sample; 
			} 

			/*Vec3f _sampleUniformHemisphere(const Vec3f& p_normal) const {
				// Point sur l'hemisphere
				const float theta = TWO_PIf * randomFloat();  
				const float phi = acos(randomFloat()); 

				Vec3f sample = glm::normalize(Vec3f(
					cos(theta) * sin(phi),
					cos(phi),
					sin(theta) * sin(phi)
				));  
				// Rotation de de l'échantillon de l'hémisphere par rapport à la normal de la surface.
				return glm::normalize(_rotateSample(sample, p_normal));
			} */
			
			const float sign(const float x) const {
				return (x > 0) - (x < 0);
			}

			Vec3f _sampleCosineWeigthedHemisphere(const Vec3f& p_normal) const {
				//const float thetaOffset = acos(p_normal.z / sqrt(p_normal.x * p_normal.x + p_normal.y * p_normal.y + p_normal.z * p_normal.z))  ;
				//const float phiOffset = sign(p_normal.x) * acos(p_normal.x / sqrt(p_normal.x * p_normal.x + p_normal.y * p_normal.y));
				// On sait que, theta est une rotation sur x et y et phi sur x et z

				// Donc nous pouvons écrire les matrix de rotation suivantes :
				// RxPhi = [(1, 0, 0), (0, cosPhi, -sinPhi), (0, sinPhi, cosPhi)]
				// RyPhi = [(cosPhi, 0, sinPhi), (0, 1, 0), (-sinPhi, 0, cosPhi)] 
				// 
				// RxTheta = [(1, 0, 0), (0, cosTheta, -sinTheta), (0, sinTheta, cosTheta)]
				// RzTheta = [(cosTheta, -sinTheta, 0), (sinTheta, cosTheta, 0), (0, 0, 1)]
				// 
				// On applique les 4 rotations 
				// RxTheta * RzTheta * RyPhi * RxPhi * sample

				const float theta = TWO_PIf * randomFloat() ;
				const float phi = acos(sqrt(randomFloat())) ; 

				const Vec3f sample = glm::normalize(Vec3f(
					cos(theta) * sin(phi),
					cos(phi),
					sin(theta) * sin(phi)
				));
				return glm::normalize(_rotateSample(sample, p_normal));
			}

			const int _nbIndirectSample = 3;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PATH_TRACING_INTEGRATOR__
