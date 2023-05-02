#ifndef __RT_ISICG_TRIANGLE_GEOMETRY__
#define __RT_ISICG_TRIANGLE_GEOMETRY__

#include "base_geometry.hpp"
//#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	class MeshTriangle;

	class TriangleMeshGeometry : public BaseGeometry
	{
	  public:
		TriangleMeshGeometry()			= delete;
		virtual ~TriangleMeshGeometry() = default;

		TriangleMeshGeometry( const unsigned int p_v0,
							  const unsigned int p_v1,
							  const unsigned int p_v2,
							  MeshTriangle *	 p_refMesh );

		bool intersect( const Ray & p_ray, float & p_t, float & p_u, float & p_v ) const;
		bool intersect( const Ray & p_ray ) const;

		Vec3f getNormalInterpolation(const float & p_u, const float & p_v) const;

		inline const Vec3f & getFaceNormal() const { return _faceNormal; }  
		const Vec3f & getVertex( const unsigned int idV ) const;
		const Vec3f& getCenter() const;

		MeshTriangle* getRefMesh();
	  private:
		MeshTriangle * _refMesh = nullptr;
		union
		{
			struct
			{
				unsigned int _v0, _v1, _v2;
			};
			unsigned int _v[ 3 ] = { 0, 0, 0 };
		};

		Vec3f _faceNormal;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_TRIANGLE_GEOMETRY__
