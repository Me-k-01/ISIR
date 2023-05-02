#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}
    Vec3f TriangleMeshGeometry::getNormalInterpolation(const float & p_u, const float & p_v) const { 
		const Vec3f & n0 = _refMesh->_normals[_v0];
		const Vec3f & n1 = _refMesh->_normals[_v1];
		const Vec3f & n2 = _refMesh->_normals[_v2];
		return normalize((1 - p_u - p_v) * n0 + p_u * n1 + p_v * n2);
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		const float EPS = 0.0000001f;
		float a, f;
        Vec3f edge1 = v1 - v0;
        Vec3f edge2 = v2 - v0;
        Vec3f h, s, q;
        h = cross(d, edge2);
        a = dot(edge1, h);
        if (a > -EPS && a < EPS)
            return false;    // This ray is parallel to this triangle.
        f = 1.0f/a;
        s = o - v0;
        p_u = f * dot(s, h);
        if (p_u < 0.0f || p_u > 1.0f)
            return false;
        q = cross(s, edge1);
        p_v = f * dot(d, q);
        if (p_v < 0.0f || p_u + p_v > 1.0f)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        const float t = f * dot(edge2, q);
        if (t > EPS) // ray intersection
        {
            // avec t tel que : point = o + d * t;
            p_t = t;
            return true;
        } else { // This means that there is a line intersection but not a ray intersection.
            p_t = -1.f;
            return false;
        }
	}  

    const Vec3f & TriangleMeshGeometry::getVertex( const unsigned int idV ) const { 
        return _refMesh->_vertices[ _v[idV] ];
    }

    const Vec3f & TriangleMeshGeometry::getCenter() const {
		return (_refMesh->_vertices[ _v0 ] + _refMesh->_vertices[ _v1 ] + _refMesh->_vertices[ _v2 ]) / 3.f; 
	}

	MeshTriangle* TriangleMeshGeometry::getRefMesh() {
		return _refMesh;
	}

    bool TriangleMeshGeometry::intersect(const Ray & p_ray) const
    {
        float t, u, v;
        return intersect(p_ray, t, u, v);
    }

} // namespace RT_ISICG
