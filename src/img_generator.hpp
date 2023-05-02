#ifndef __RT_ISICG_IMAGE_GENERATOR__
#define __RT_ISICG_IMAGE_GENERATOR__


#include "cameras/perspective_camera.hpp"
#include "renderer.hpp"

namespace RT_ISICG
{
	class ImgGenerator {
		public: 
			 
			enum TP_num {
				TP1, TP2, TP3, TP4, Conference, TP5, TP5_2, TP5_GoldenBunny, TP6, TP7, Projet
			};

			ImgGenerator(const int p_imgWidth, const int p_imgHeight) : _imgWidth(p_imgWidth), _imgHeight(p_imgHeight) {
				// Create a texture to render the scene. 
			}
			virtual ~ImgGenerator() = default;
			 
			inline void setup(const TP_num & tp_num) {
				const float screenRatio = float(_imgWidth) / float(_imgHeight);
				Vec3f posCamera, dirCamera;
				switch (tp_num)
				{
				case TP1:
					_scene.initScene1();

					// Create a perspective camera.
					//_camera = new PerspectiveCamera( screenRatio );
					
					// Camera positionnable :
					posCamera = Vec3f(0.f, 0.f, -2.f); dirCamera = Vec3f(0.f, 0.f, 79.f);
					//Vec3f posCamera = Vec3f(1, 0, 0); Vec3f dirCamera = Vec3f(1, 0, 1);
					//Vec3f posCamera = Vec3f(0, 1, 0); Vec3f dirCamera = Vec3f(0, 1, 1);
					//Vec3f posCamera = Vec3f(4, -1, 0); Vec3f dirCamera = Vec3f(-1, -1, 2);
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.	
					// Antialiasing
					_renderer.setNbPixelSamples(10);
					_renderer.setIntegrator( IntegratorType::RAY_CAST ); 
					_renderer.setBackgroundColor( GREY );
					break;

				case TP2: 
					_scene.initScene2();
					posCamera = Vec3f(0.f, 0.f, -2.f); dirCamera = Vec3f(0.f, 0.f, 79.f);
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					// Create and setup the renderer.	
					// Antialiasing
					_renderer.setNbPixelSamples(10);
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING ); 
					_renderer.setBackgroundColor( GREY );
					break;

				case TP3: 
					_scene.initScene3();
					posCamera = Vec3f(0.f, 0.f, -2.f); dirCamera = Vec3f(0.f, 0.f, 79.f);

					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					// Create and setup the renderer.	 
					// Antialiasing
					_renderer.setNbPixelSamples(100);
					// Reduction du bruit sur l'ombre douce
					_renderer.setNbLightSamples(10);
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING );
					_renderer.setBackgroundColor( GREY );
					break;

				case TP4:
					_scene.initScene4();
					posCamera = Vec3f(0.f, 2.f, -7.f); dirCamera = Vec3f(0.f, 0.f, 79.f);
					// Seconde scene
					//posCamera = Vec3f(-250.f, 500.f, 330.f); dirCamera = Vec3f(0.f, 350.f, 100.f);
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.	
					_renderer.setNbPixelSamples(10);
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING );
					_renderer.setBackgroundColor( GREY );
					break;

				case TP5:
					_scene.initScene5();
					
					///// Camera
					posCamera = Vec3f(0.f); // Origine
					dirCamera = Vec3f(0.f, 0.f, 1.f); // Axe z
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					///// Create and setup the renderer.
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING );
					_renderer.setBackgroundColor( GREY );
					break;

				case TP5_2: // Blin-Phong
					_scene.initScene5_2();
					posCamera = Vec3f(0.f, 0.f, 0.f); dirCamera = Vec3f(0.f, 0.f, 1.f); 
					//posCamera = Vec3f(0.f, 0.f, 4.f); dirCamera = Vec3f(0.f, -4.f, 10.f); 

					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING );
					_renderer.setBackgroundColor( GREY );
					break;

				case TP5_GoldenBunny:
					
					_scene.initScene5_4(); 
					posCamera = Vec3f(0.f, 0.f, -6.f); dirCamera = Vec3f(0.f, 0.f, 79.f);
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.
					_renderer.setNbPixelSamples(20);
					_renderer.setIntegrator( IntegratorType::DIRECT_LIGHTING ); 
					_renderer.setBackgroundColor( GREY );
					break;

				case TP6:
					_scene.initScene6(); 
					posCamera = Vec3f(0.f, 2.f, -6.f); dirCamera = Vec3f(0.f, 2.f, 0.f); 
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.
					_renderer.setNbPixelSamples(10);
					_renderer.setNbLightSamples(10);
					_renderer.setIntegrator( IntegratorType::WHITTED ); 
					_renderer.setBackgroundColor( GREY );
					break; 

				case TP7:
					_scene.initScene7();  
					posCamera = Vec3f(5.40539f, 6.01505f, -6.81196f); dirCamera = Vec3f(0.f, 0.f, 0.f); 
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);

					// Create and setup the renderer.
					_renderer.setNbPixelSamples(10);
					_renderer.setNbLightSamples(100);
					_renderer.setIntegrator( IntegratorType::WHITTED ); 
					_renderer.setBackgroundColor( GREY );
					break;

				case Conference:
					_scene.initSceneConference(); 
					posCamera = Vec3f(250.f, 500.f, 330.f); dirCamera = Vec3f(0.f, 350.f, 100.f); 
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					// Create and setup the renderer.	
					_renderer.setNbPixelSamples( 1 );
					_renderer.setNbLightSamples( 1 );
					_renderer.setIntegrator( IntegratorType::RAY_CAST );
					_renderer.setBackgroundColor( GREY );
					break;

				case Projet: default:
					_scene.initSceneProject(); 
					posCamera = Vec3f(0.f, 2.f, -6.f); dirCamera = Vec3f(0.f, 2.f, 0.f); 
					_camera = new PerspectiveCamera(posCamera, dirCamera, Vec3f(0.f, 1.f, 0.f), 60.f, screenRatio);
					
					// Create and setup the renderer.
					_renderer.setNbPixelSamples(10);
					_renderer.setNbLightSamples(10);
					_renderer.setIntegrator( IntegratorType::WHITTED ); 
					_renderer.setBackgroundColor( BLACK );
					break;  
				}
			}

			inline void generate(const std::string imgName) {
				Texture img( _imgWidth, _imgHeight );
				// Launch rendering.
				std::cout << "Rendering..." << std::endl;
				std::cout << "- Image size: " << _imgWidth << "x" << _imgHeight << std::endl;

				float renderingTime = _renderer.renderImage( _scene,  _camera, img );

				std::cout << "-> Done in " << renderingTime << "ms" << std::endl;
				// Save rendered image. 
				img.saveJPG( RESULTS_PATH + imgName );
			}

		private:
			int _imgWidth;
			int _imgHeight;
			PerspectiveCamera* _camera = nullptr; 
			Scene _scene;
			Renderer _renderer;

			
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE_GENERATOR__
