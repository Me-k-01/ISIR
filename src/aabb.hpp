#ifndef __RT_ISICG_AABB__
#define __RT_ISICG_AABB__

#include "ray.hpp"

namespace RT_ISICG
{
	struct AABB
	{
	  public:
		AABB() = default;
		AABB( const Vec3f & p_min, const Vec3f & p_max ) : _min( p_min ), _max( p_max ) {}
		~AABB() = default;

		inline const Vec3f & getMin() const { return _min; }
		inline const Vec3f & getMax() const { return _max; }
		inline const void setMin(Vec3f p_min) { _min = p_min; }
		inline const void setMax(Vec3f p_max) { _max = p_max; }

		// Returns true if the AABB is degenerated.
		inline bool isValid() const { return ( ( _min.x <= _max.x ) && ( _min.y <= _max.y ) && ( _min.z <= _max.z ) ); }

		// Extends the AABB with a point
		inline void extend( const Vec3f & p_point )
		{ 
			// Extension de min
			if (_min.x > p_point.x) {
				_min.x = p_point.x;
			} 
			if (_min.y > p_point.y) {
				_min.y = p_point.y;
			} 
			if (_min.z > p_point.z) {
				_min.z = p_point.z;
			}
			// Extension de max
			if (_max.x < p_point.x) {
				_max.x = p_point.x;
			} 
			if (_max.y < p_point.y) {
				_max.y = p_point.y;
			} 
			if (_max.z < p_point.z) {
				_max.z = p_point.z;
			}
		
		}
		// Extends the AABB with another AABB
		inline void extend( const AABB & p_aabb )
		{
			Vec3f otherMin = p_aabb.getMin();
			Vec3f otherMax = p_aabb.getMax();
			// Extension de min
			if (_min.x > otherMin.x) {
				_min.x = otherMin.x;
			}  
			if (_min.y > otherMin.y) {
				_min.y = otherMin.y;
			}  
			if (_min.z > otherMin.z) {
				_min.z = otherMin.z;
			}  
			// Extension de max
			if (_max.x < otherMax.x) {
				_max.x = otherMax.x;
			}  
			if (_max.y < otherMax.y) {
				_max.y = otherMax.y;
			}  
			if (_max.z < otherMax.z) {
				_max.z = otherMax.z;
			}  
		}
		 
		// Returns the AABB diagonal vector.
		inline Vec3f diagonal() const { return _max - _min; }

		// Returns the AABB centroid.
		inline Vec3f centroid() const { return ( _min + _max ) * 0.5f; }

		// Returns the largest axis, 0 -> x, 1 -> y, 2 -> z
		inline size_t largestAxis() const
		{
			const Vec3f d = diagonal();
			if ( d.x > d.y && d.x > d.z )
				return 0;
			else if ( d.y > d.z )
				return 1;
			else
				return 2;
		}

		bool intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const;

	  private:
		Vec3f _min = Vec3f( FLT_MAX );
		Vec3f _max = Vec3f( -FLT_MAX );
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_AABB__
