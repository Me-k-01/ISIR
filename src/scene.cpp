#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "objects/triangle_mesh.hpp"
#include "objects/implicit_sphere.hpp"
#include "objects/implicit_torus.hpp"
#include "objects/implicit_box.hpp"
#include "objects/implicit_octahedron.hpp"

#include "lights/quad_light.hpp"
#include "lights/point_light.hpp"
#include "lights/sphere_light.hpp"
#include "lights/torus_light.hpp"

#include "materials/lambert_material.hpp"
#include "materials/matte_material.hpp"
#include "materials/plastic_material.hpp" 
#include "materials/microfacet_material.hpp" 
#include "materials/mirror_material.hpp" 
#include "materials/transparent_material.hpp" 

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::initScene1()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
	}

	void Scene::initScene2()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f )) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight(new PointLight(Vec3f(1.f, 10.f, 1.f), WHITE, 100.f));
	}
	void Scene::initScene3()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f )) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight( new QuadLight( 
			Vec3f( 1.f, 10.f, 2.f ),  // pos
			Vec3f(-2.f, 0.f, 0.f ), // u
			Vec3f( 0.f, 0.f, 2.f ), // v
			WHITE, // col
			40.f
		));
	
	}
	
	void Scene::initScene4()
	{
		const std::string DATA_PATH = "./data/";

        // = = = = = = = = = Add materials . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
        _addMaterial( new ColorMaterial( "RedColor", RED ) );
        _addMaterial( new ColorMaterial( "GreenColor", GREEN ) );
        _addMaterial( new ColorMaterial( "BlueColor", BLUE ) );
        _addMaterial( new ColorMaterial( "GreyColor", GREY ) );
        _addMaterial( new ColorMaterial( "MagentaColor", MAGENTA ) );
        _addMaterial( new ColorMaterial( "YellowColor", YELLOW ) );
        _addMaterial( new ColorMaterial( "CyanColor", CYAN ) );
        //_addMaterial( new MicrofacetMaterial("Gold", Vec3f(1.f, 0.85f, 0.57f), 0.5f , 0.3f, Vec3f(1.f, 0.85f, 0.57f)) );
	
        // = = = = = = = = = Add objects . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
        //loadFileTriangleMesh( "UVsphere", DATA_PATH + "uvsphere.obj" );
        loadFileTriangleMesh( "UVsphere", DATA_PATH + "Bunny.obj" );
        _attachMaterialToObject( "CyanColor", "UVsphere_defaultobject" );
        //_attachMaterialToObject( "Gold", "UVsphere_defaultobject" ); 
		
        // Pseudo Cornell box made with infinite planes .
        _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
        _attachMaterialToObject( "GreyColor", "PlaneGround" );
        _addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
        _attachMaterialToObject( "RedColor", "PlaneLeft" );
        _addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
        _attachMaterialToObject( "GreenColor", "PlaneCeiling" );
        _addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
        _attachMaterialToObject( "BlueColor", "PlaneRight" );
        _addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
        _attachMaterialToObject( "MagentaColor", "PlaneFront" );
        _addObject( new Plane( "PlaneRear", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
        _attachMaterialToObject( "YellowColor", "PlaneRear" );

        // = = = = = = = = = Add lights . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
        _addLight( new PointLight( Vec3f( 0.f, 3.f, -5.f ), WHITE, 100.f ) );
	}

	void Scene::initSceneConference() {
		
		const std::string DATA_PATH = "./data/";
		loadFileTriangleMesh( "Conf", DATA_PATH + "conference/conference.obj" );
		//_addLight( new PointLight( Vec3f( 900, 600, -300 ), WHITE, 100.f ) );
		
		_addLight(new QuadLight(Vec3f(900.f, 600.f, -300.f), Vec3f(-800.f, 0.f, 0.f ), Vec3f(0.f, 1.f, 300.f), WHITE, 40.f));
	}

	
	void Scene::initScene5()
	{
		// Ajout des objets
		_addObject( new Sphere( "Sphere", Vec3f( 0.f, 0.f, 3.f ), 1.f ) ); 
        
		//_addMaterial( new LambertMaterial("Grey", GREY));
		//_addMaterial( new MatteMaterial("Grey", GREY, 0.4f));
		

		//_addMaterial( new PlasticMaterial("Grey", GREY, GREY, 8.f)); 
		_addMaterial( new PlasticMaterial("Grey", GREY, GREY, 16.f));  
		
		_attachMaterialToObject( "Grey", "Sphere" ); 
		
		_addMaterial( new LambertMaterial("Red", RED));  
        _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) ); // Orienté en y
		//addMaterial( new PlasticMaterial("Red", RED, RED, 64.f));  // TP5 - 5 : Correction phong
        _attachMaterialToObject( "Red", "PlaneGround" );

        _addLight( new PointLight( Vec3f( 0.f, 0.f, -2.f ), WHITE, 60.f ) );
	}

	void Scene::initScene5_2() // Correction de Phong avec le model de Blinn-Phong
	{ 
		// Ajout des objets
		//_addObject( new Sphere( "Obj", Vec3f( 0.f, 0.f, 3.f ), 1.f ) ); 
        
		//_addMaterial( new LambertMaterial("Grey", GREY));
		//_addMaterial( new MatteMaterial("Grey", GREY, 0.4f));
		//_addMaterial( new PlasticMaterial("Grey", GREY, 0.3f, 64.f));
		//_addMaterial( new MicrofacetMaterial("Gold", Vec3f(1.f, 0.85f, 0.57f), 0.5f , 0.3f));
		//_attachMaterialToObject( "Gold", "Obj" ); 
		
        _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -0.1f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) ); 
		//_addMaterial( new LambertMaterial("Red", RED));  
		_addMaterial( new PlasticMaterial("Red", BLACK, RED, 64.f));  // TP5 - 5 : Correction phong
        _attachMaterialToObject( "Red", "PlaneGround" );

        // Ajout des lumières 
        _addLight( new PointLight( Vec3f( 0.f, 1.5f, 5.f ), WHITE, 60.f ) );   // TP5 - 5 : Correction phong
	}

	void Scene::initScene5_4() { // Golden bunny !
		
		const std::string DATA_PATH = "./data/";
		// Ajout des objets
        loadFileTriangleMesh( "UVsphere", DATA_PATH + "Bunny.obj" );

		_addMaterial( new MicrofacetMaterial("Gold", Vec3f(1.f, 0.85f, 0.57f), 0.5f , 0.3f)); 
        _attachMaterialToObject( "Gold", "UVsphere_defaultobject" );
		
        _addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) ); 
		_addMaterial( new LambertMaterial("Red", RED));  
        _attachMaterialToObject( "Red", "PlaneGround" );
        // Ajout des lumières
        _addLight( new PointLight( Vec3f( 0.f, 5.f, -5.f ), WHITE, 100.f ) );
	}

	void Scene::initScene6() {
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add materials .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		_addMaterial( new MatteMaterial ( "WhiteMatte", WHITE, 0.6f ));
		_addMaterial( new MatteMaterial ( "RedMatte", RED, 0.6f ));
		_addMaterial( new MatteMaterial ( "GreenMatte", GREEN, 0.6f ));
		_addMaterial( new MatteMaterial ( "BlueMatte", BLUE, 0.6f ));
		_addMaterial( new MatteMaterial ( "GreyMatte", GREY, 0.6f ));
		_addMaterial( new MatteMaterial ( "MagentaMatte", MAGENTA, 0.6f )); 
		_addMaterial( new MirrorMaterial( "Mirror" ));
		_addMaterial( new TransparentMaterial( "Transparent" ));

		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add objects .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		
		// Spheres .
		_addObject( new Sphere( "Sphere1" , Vec3f( -2.f , 0.f , 3.f ) , 1.5f ));
		_attachMaterialToObject( "Mirror" , "Sphere1" );
		_addObject ( new Sphere ( "Sphere2" , Vec3f( 2.f , 0.f , 3.f ) , 1.5f ));
		_attachMaterialToObject("Transparent" , "Sphere2" );
		// Pseudo Cornell box made with infinite planes .
		_addObject ( new Plane( "PlaneGround" , Vec3f( 0.f , -3.f , 0.f ), Vec3f( 0.f , 1.f , 0.f )));
		_attachMaterialToObject( "GreyMatte" , "PlaneGround" );
		_addObject ( new Plane( "PlaneLeft" , Vec3f( 5.f , 0.f , 0.f ), Vec3f( -1.f , 0.f , 0.f )));
		_attachMaterialToObject( "RedMatte" , "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling" , Vec3f( 0.f , 7.f , 0.f ) , Vec3f( 0.f , -1.f , 0.f )));
		_attachMaterialToObject( "GreenMatte" , "PlaneCeiling" );
		_addObject( new Plane ( "PlaneRight" , Vec3f( -5.f , 0.f , 0.f ), Vec3f( 1.f , 0.f , 0.f )));
		_attachMaterialToObject("BlueMatte", "PlaneRight");
		_addObject( new Plane( "PlaneFront" , Vec3f ( 0.f , 0.f , 10.f ), Vec3f( 0.f , 0.f , -1.f )));
		_attachMaterialToObject( "MagentaMatte" , "PlaneFront" );

		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add lights .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		//_addLight( new PointLight( Vec3f ( 0.f , 5.f , 0.f ), WHITE, 100.f ));
		_addLight( new QuadLight( Vec3f ( 1.f , 5.f , -2.f ) , Vec3f( -2.f , 0.f , 0.f ) , Vec3f( 0.f , 1.f , 2.f ) , WHITE , 40.f ) );
	}

	void Scene::initScene7() {
        //_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) ); 
		_addMaterial( new MatteMaterial( "WhiteMatte", WHITE, 0.6f ));
		_addMaterial( new MatteMaterial( "RedMatte", RED, 0.6f ));
		_addMaterial( new MatteMaterial( "GreenMatte", GREEN, 0.6f ));
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.6f ));
		_addMaterial( new MatteMaterial( "GreyMatte", GREY, 0.6f ));
		_addMaterial( new MatteMaterial( "MagentaMatte", MAGENTA, 0.6f ));
		
        _addObject( new Plane( "PlaneGround", Vec3f( 0.f, 0.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) ); 
		_addObject( new ImplicitSphere( "Sphere", Vec3f( 1.82257f, 1.f, 0.f ), 1.f ) ); 
		_addObject( new ImplicitBox( "Box", Vec3f( 0.f, 0.986899f, -2.56195f ), Vec3f(1.f) ) ); 
		_addObject( new ImplicitTorus( "Torus", Vec3f( -0.523258f, 1.7264f, -0.547237f ), .6f, .2f ) );
		_addObject( new ImplicitOctahedron( "Octahedron", Vec3f( 2.21068f, 1.634478f, -4.077129f ), 0.5f ) );  
		
        _attachMaterialToObject( "MagentaMatte", "PlaneGround" );
		_attachMaterialToObject( "WhiteMatte" , "Sphere" );  
		_attachMaterialToObject( "GreenMatte" , "Box" );  
		_attachMaterialToObject( "GreyMatte" , "Octahedron" );  
		_attachMaterialToObject( "BlueMatte" , "Torus" );   

        // Ajout des lumières
        _addLight( new PointLight( Vec3f( -1.21859f, 1.23233f, -2.80147f ), WHITE, 20.f ) );
        _addLight( new PointLight( Vec3f( 2.82403f, 1.23233f, 1.20782f ), WHITE, 20.f ) ); 
        _addLight( new PointLight( Vec3f( 2.80403f, 2.81158f, -3.11593f ), Vec3f(0.6f, 0.6f, 1.f ), 20.f));
	}
	
	void Scene::initSceneProject() {
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add materials .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		_addMaterial( new MatteMaterial ( "WhiteMatte", WHITE, 0.6f ));
		_addMaterial( new MatteMaterial ( "RedMatte", RED, 0.6f ));
		_addMaterial( new MatteMaterial ( "GreenMatte", GREEN, 0.6f ));
		_addMaterial( new MatteMaterial ( "BlueMatte", BLUE, 0.6f ));
		_addMaterial( new MatteMaterial ( "GreyMatte", GREY, 0.6f ));
		_addMaterial( new MatteMaterial ( "MagentaMatte", MAGENTA, 0.6f )); 
		_addMaterial( new MirrorMaterial( "Mirror" ));
		_addMaterial( new TransparentMaterial( "Transparent" ));
#ifdef TEST_TORUS
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add objects .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Spheres .
		//_addObject( new Sphere( "Sphere1" , Vec3f( -2.f , 0.f , 3.f ) , 1.5f ));
		//_attachMaterialToObject( "WhiteMatte" , "Sphere1" );
		//_addObject ( new Sphere ( "Sphere2" , Vec3f( 2.f , 0.f , 3.f ) , 0.5f ));
		//_attachMaterialToObject( "WhiteMatte" , "Sphere2" );
		//_attachMaterialToObject("WhiteMatte" , "SphereLight" );
		//_addLight( new SphereLight( Vec3f( 2.f , 0.f , 3.f ) , 0.6f, WHITE, 10.f ));
		
		_addObject( new Sphere( "Sphere1" , Vec3f( -2.f , 0.f , 3.f ) , 1.5f ));
		_attachMaterialToObject("Mirror" , "Sphere1" ); 

		//_addObject ( new ImplicitTorus( "Torus" , Vec3f( 2.f , 0.f , 3.f ) , 1.5f, 0.3f));
		//_attachMaterialToObject("WhiteMatte" , "Torus" ); 
		
		_addObject ( new Sphere ( "Sphere2" , Vec3f( 2.f , 0.f , 3.f ) , 0.5f ));
		_attachMaterialToObject("Mirror" , "Sphere2" );
		/*
		_addObject ( new Sphere ( "SphereLight" , Vec3f( 2.f , 0.f , 3.f ) , 0.5f )); 
		_attachMaterialToObject( "Transparent" , "Sphere1" ); 
		_addLight( new SphereLight( Vec3f( 2.f , 0.f , 3.f ) , 0.6f, WHITE, 10.f ));
		*/
		// Pseudo Cornell box made with infinite planes .
		_addObject ( new Plane( "PlaneGround" , Vec3f( 0.f , -3.f , 0.f ), Vec3f( 0.f , 1.f , 0.f )));
		_attachMaterialToObject( "GreyMatte" , "PlaneGround" );
		_addObject ( new Plane( "PlaneLeft" , Vec3f( 5.f , 0.f , 0.f ), Vec3f( -1.f , 0.f , 0.f )));
		_attachMaterialToObject( "RedMatte" , "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling" , Vec3f( 0.f , 7.f , 0.f ) , Vec3f( 0.f , -1.f , 0.f )));
		_attachMaterialToObject( "GreenMatte" , "PlaneCeiling" );
		_addObject( new Plane ( "PlaneRight" , Vec3f( -5.f , 0.f , 0.f ), Vec3f( 1.f , 0.f , 0.f )));
		_attachMaterialToObject("BlueMatte", "PlaneRight");
		_addObject( new Plane( "PlaneFront" , Vec3f ( 0.f , 0.f , 10.f ), Vec3f( 0.f , 0.f , -1.f )));
		_attachMaterialToObject( "MagentaMatte" , "PlaneFront" );

		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// Add lights .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		//_addLight( new PointLight( Vec3f ( 0.f , 5.f , 0.f ), WHITE, 100.f ));
		_addLight( new SphereLight( Vec3f ( 2.f , 0.f , 3.f ), 0.6f, WHITE, 10.f ));
		//_addLight( new QuadLight( Vec3f ( 1.f , 5.f , -2.f ) , Vec3f( -2.f , 0.f , 0.f ) , Vec3f( 0.f , 1.f , 2.f ) , WHITE , 40.f ) ); 
		//_addLight( new TorusLight(Vec3f( 2.f , 0.f , 3.f ) , 1.5f, 0.4f, WHITE , 10.f ));
	
#else 
		const std::string DATA_PATH = "./data/";
		loadFileTriangleMesh( "Conf", DATA_PATH + "conference/conference.obj" );

		//_addObject( new Sphere( "SphereLight1" , Vec3f( 589.f , 519.f, 97.f ) , 20.f ));
		//_attachMaterialToObject("WhiteMatte" , "SphereLight1" ); 
		//_addLight( new SphereLight( Vec3f( 589.f , 519.f, 97.f), 21.f, WHITE, 1000.f ));

		
		//_addObject( new Sphere( "SphereLight2" , Vec3f( 189.f , 519.f, 97.f), 20.f ));
		//_attachMaterialToObject("WhiteMatte" , "SphereLight2" ); 
		//_addLight( new SphereLight( Vec3f( 189.f , 519.f, 97.f), 21.f, WHITE, 1000.f ));
		
		// Exit-sign light
		//_addLight( new QuadLight( Vec3f ( -284.29f , 536.801f , -561.784f ) , Vec3f( -71.2235f , 0.f , 0.f ), Vec3f( 0.f, 47.8f, 0.f)  , GREEN , 40.f ) );
		//_addLight( new QuadLight( Vec3f ( -284.29f , 536.801f , -561.784f ) , Vec3f( 0.f, 47.8f, 0.f ) , Vec3f( -71.2235f , 0.f , 0.f) , GREEN , 40.f ) );
		 
		// Mirror
		_addObject( new ImplicitBox( "MirrorBox", Vec3f( -338.9f, 337.576f, 270.275f ), Vec3f(41, 595, 345) ) ); 
		_attachMaterialToObject( "Mirror" , "MirrorBox" );
		
		_attachMaterialToObject("Transparent", "Conf_mesh29");
		_addLight(new QuadLight(Vec3f(900.f, 600.f, -300.f), Vec3f(-800.f, 0.f, 0.f ), Vec3f(0.f, 1.f, 300.f), WHITE, 40.f));
#endif // TEST_TORUS 
	}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			_addObject( triMesh );
			triMesh->loadBVH();

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				//_addMaterial( new ColorMaterial( std::string( mtlName.C_Str() ), kd ) );
				_addMaterial( new PlasticMaterial( std::string( mtlName.C_Str() ), kd, ks, s ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
				//std::cout << "------> attached to : " << meshName << "\n";
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::_addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			std::cout << "Material \'" << name << "\' added." << std::endl;
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
