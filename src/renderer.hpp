#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/base_camera.hpp"
#include "defines.hpp"
#include "integrators/base_integrator.hpp" 
#include "texture.hpp"
#include "utils/chrono.hpp"
#include <omp.h>

namespace RT_ISICG
{
	class Renderer
	{
	  public:
		Renderer();
		~Renderer() { delete _integrator; }

		void setIntegrator( const IntegratorType p_integratorType ); 
		void setBackgroundColor( const Vec3f & p_color );

		inline void setNbPixelSamples( const int p_nbPixelSamples ) { _nbPixelSamples = p_nbPixelSamples; }
		inline void setNbLightSamples( const int p_nbLightSamples ) { _nbLightSamples = p_nbLightSamples; } // A appeller avant setIntegrator

		float renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture ); 

	  private:
		BaseIntegrator * _integrator	 = nullptr;
		int				 _nbPixelSamples = 1; // Antialiasing
		int				 _nbLightSamples = 1; // Lumière surfacique
		int				 _nbLightBounces = 2; // Rebond mirroir / indirect lighting 
		int				 _nbIndirectLightSample = 3; //  Pour approximation de l'intégral de Monte Carlo
		inline float _getOffset() const;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
