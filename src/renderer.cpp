#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include "integrators/path_tracing_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch (p_integratorType)
		{
			case IntegratorType::DIRECT_LIGHTING:
			{
				_integrator = new DirectLightingIntegrator(_nbLightSamples);
				break;
			}
			case IntegratorType::WHITTED: 
			{
				_integrator = new WhittedIntegrator(_nbLightSamples, _nbLightBounces);
				break;
			}
			case IntegratorType::PATH_TRACED: 
			{
				_integrator = new PathTracingIntegrator(_nbLightSamples, _nbIndirectLightSample, _nbLightBounces);
				break;
			}
			case IntegratorType::RAY_CAST:
			//default:
			{
				_integrator = new RayCastIntegrator();
				break;
			}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	inline float Renderer::_getOffset() const {
		// Retourne un nombre entre 0 et 1 lorsqu'il y a plus d'un rayon
		// Sinon retourne 0.5, pour tirer au milieu du pixel
		return _nbPixelSamples == 1 ? 0.5f : randomFloat();
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();
		
		#pragma omp parallel for
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				//p_texture.setPixel(i, j, (ray.getDirection() + 1.f) * 0.5f );
				Vec3f sumRGB = Vec3f(0.f); // somme des echantillons de couleurs du pixel
				for (int k = 0; k < _nbPixelSamples; k++) {
					float x = (float(i) + _getOffset()) / (float)width; // On ajoute 0.5 pour tirer au milieu du pixel, et on retire 0.5 pour le random float
					float y = (float(j) + _getOffset()) / (float)height; 
					Ray ray = p_camera->generateRay(x, y);
					sumRGB += glm::clamp(_integrator->Li(p_scene, ray, 0.f, 10000.f), Vec3f(0.f), Vec3f(1.f));
				}
				p_texture.setPixel(i, j, sumRGB / Vec3f(_nbPixelSamples));
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
