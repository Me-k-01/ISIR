#include "bvh.hpp"
#include "utils/chrono.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();
		_root = new BVHNode();  
		_buildRec(_root, 0, p_triangles->size(), 1);

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		return _intersectRec(_root, p_ray, p_tMin, p_tMax, p_hitRecord); 
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec(_root, p_ray, p_tMin, p_tMax);

	}
	 

	void BVH::_buildRec( BVHNode *			p_node, // Parent
						 unsigned int p_firstTriangleId, // Indice du premier triangle
						 unsigned int p_lastTriangleId, // Indice du dernier triangle
						 const unsigned int p_depth )
	{ 
		 
		// ----- Calcul du AABB pour ce noeud -----
		for (unsigned int i = p_firstTriangleId; i < p_lastTriangleId; i++) {
			TriangleMeshGeometry tri = _triangles->at(i);

			Vec3f v0, v1, v2;
			tri.getVertex(v0, v1, v2);
			//std::cout << v0[1]  << "," << v0[1] << ","  << v0[2];
			p_node->_aabb.extend(v0);
			p_node->_aabb.extend(v1);
			p_node->_aabb.extend(v2);
		}
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId = p_lastTriangleId;
		//std::cout << "build first " << p_firstTriangleId << "\n";
		//std::cout << "build last " << p_lastTriangleId << "\n";
		
		// ----- Critère d'arrêt -----
		// Soit on a atteint la profondeur maximal, soit il n'y a p^lus assez de triangle dans la boîte pour découpper
		if (p_depth >= _maxDepth || p_lastTriangleId - p_firstTriangleId <= _maxTrianglesPerLeaf) { 
			return;
		}
		
		const Vec3f parentMin = p_node->_aabb.getMin();
		const Vec3f parentMax = p_node->_aabb.getMax();
		Vec3f length = parentMax - parentMin;  

		// ----- On choisit l'axe de la partition -----
		Vec3f middle = Vec3f(parentMin);
		std::function<bool(TriangleMeshGeometry)> splitFunction;
		
		if (length.x > length.y) {
			// Axe x plus grand 
			middle.x += length.x / 2.f;
			splitFunction = [middle](TriangleMeshGeometry tri) -> bool {
				return tri.getCenter().x < middle.x; // Séparation selon x
			};
		} else {
			// Axe y plus grand
			middle.y += length.y / 2.f;
			splitFunction = [middle](TriangleMeshGeometry tri) -> bool {
				return tri.getCenter().y < middle.y; // Séparation selon y
			};
		}
		 
		// On effectue la partition (séparation du tableau de firstTriId à lastTriId selon un des deux axes)
		std::vector<TriangleMeshGeometry>::iterator iteratorMiddle = std::partition(
			_triangles->begin() + p_firstTriangleId, 
			_triangles->begin() + p_lastTriangleId, 
			splitFunction
		);

		// Indice du milieu de la séparation
		const unsigned int idPartition = std::distance(_triangles->begin(), iteratorMiddle); 
 
		// ----- Création des noeuds enfants -----
		p_node->_left = new BVHNode();
		p_node->_left->_aabb.setMin(parentMin);
		p_node->_left->_aabb.setMax(middle);

		p_node->_right = new BVHNode();
		p_node->_right->_aabb.setMin(middle);
		p_node->_right->_aabb.setMax(parentMax);

		// On continue la construction avec les enfants nouvellement créés
		_buildRec(p_node->_left, p_firstTriangleId, idPartition, p_depth + 1);
		_buildRec(p_node->_right, idPartition, p_lastTriangleId, p_depth + 1);

	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		// ----- Si le rayon ne passe pas par la boîte englobante -----
		// on arrêtte d'explorer cette branche
		if (! p_node->_aabb.intersect(p_ray, p_tMin, p_tMax)) { 
			return false;
		} 

		// ----- Si on est sur une feuille -----
		// On test l'intesection avec les triangles contenus dans la boîte englobante
		if (p_node->isLeaf()) { 
			for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++) { 
				float t, u, v;
				if (_triangles->at(i).intersect(p_ray, t, u, v)) { // On a une intersection avec un des triangles 
					if (p_tMin <= t && t <= p_tMax) { // On verifie qu'on soit dans le frustum 
						// ----- On compare le hit record ----- 
						// On update le hit record uniquement si la nouvelle intersection est plus proche
						if (p_hitRecord._object != nullptr && t > p_hitRecord._distance) 
							continue; // On skip donc l'intersection si elle est moins proche de la caméra que celle enregistré actuellement dans le hit record
						
						// On update le hit record
						TriangleMeshGeometry tri = _triangles->at(i);
						p_hitRecord._point	= p_ray.pointAtT( t );
						p_hitRecord._normal = tri.getNormalInterpolation(u, v);
						p_hitRecord.faceNormal( p_ray.getDirection() );
						p_hitRecord._distance = t;
						p_hitRecord._object	  = reinterpret_cast<BaseObject *>(tri.getRefMesh());
					}
				}
			}
			return p_hitRecord._object != nullptr; 
		} // Sinon, il reste au moins un enfant.
		
		// ----- On explore donc la/les branches enfants -----
		if (p_node->_left != nullptr) {
			_intersectRec(p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord);
		}		
		if (p_node->_right != nullptr) {
			_intersectRec(p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord);
		}
		return p_hitRecord._object != nullptr;
			 
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		
		// Si le rayon ne passe pas par la boite englobante, on arrêtte d'explorer cette branche
		if (! p_node->_aabb.intersect(p_ray, p_tMin, p_tMax)) { 
			return false;
		}

		// Si on est sur une feuille, test l'intesection avec les triangles contenu dans la boite englobante
		if (p_node->isLeaf()) { 
			for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++) {
				float t, u, v;
				if (_triangles->at(i).intersect(p_ray, t, u, v)) {  // On a une intersection avec un des triangles
					if (p_tMin <= t && t <= p_tMax) // On verifie qu'on soit dans le frustum
						return true; 
				}
			}
			return false; // Aucune intersection avec les triangles de cette boite englobante
		} // Sinon, il reste au moins un enfant.
		
		// ----- On explore donc la/les branches enfants -----
		if (p_node->_left != nullptr) {
			if (_intersectAnyRec(p_node->_left, p_ray, p_tMin, p_tMax)) {
				return true;
			}
		}		
		if (p_node->_right != nullptr) {
			if (_intersectAnyRec(p_node->_right, p_ray, p_tMin, p_tMax)) {
				return true;
			}
		}

		// On n'a pas trouvé d'intersection
		return false;
	}
} // namespace RT_ISICG
