#include "triangle_mesh.hpp"

namespace RT_ISICG
{

	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		//if (!_aabb.intersect(p_ray, p_tMin, p_tMax))
		//	return false;
		return _bvh.intersect(p_ray, p_tMin, p_tMax, p_hitRecord);

		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
			
		float u, v;
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			float uCurr, vCurr;
			if ( _triangles[ i ].intersect( p_ray, t, uCurr, vCurr) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					tClosest = t;
					hitTri	 = i;
					u = uCurr;
					v = vCurr;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _triangles[ hitTri ].getNormalInterpolation(u, v);
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		//if (!_aabb.intersect(p_ray, p_tMin, p_tMax))
		//	return false;
		
		return _bvh.intersectAny(p_ray, p_tMin, p_tMax);

		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t, u, v;
			if ( _triangles[ i ].intersect( p_ray, t, u, v) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
	void MeshTriangle::loadBVH(){
		_bvh.build( & _triangles);
	}
} // namespace RT_ISICG
