#include "aabb.hpp"

namespace RT_ISICG
{
	inline float maxi(const Vec3f & v) {
		return std::max(v.x, std::max(v.y, v.z));
	}
	inline float mini(const Vec3f & v) {
		return std::min(v.x, std::min(v.y, v.z));
	}
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		// Naif : On lance le rayon sur chaque face de la boite englobante

		// On test l'inéquation que nous donne le rayon lorsqu'on inject les points min et max 
		const Vec3f & o = p_ray.getOrigin();
		const Vec3f & d = p_ray.getDirection();

		Vec3f tMin = (_min - o) / d;
		Vec3f tMax = (_max - o) / d;
		Vec3f inf = min(tMin, tMax); 
		Vec3f sup = max(tMin, tMax);
		float tNear = maxi(inf);
		float tFar = mini(sup);

		return tNear <= tFar;

		/*
		// Une autre methode serait de tester pour chaque axe
		Vec3f inf, sup;
		inf.x = (_min.x - o.x) / d.x;
		sup.x = (_max.x - o.x) / d.x;
		if (inf.x > sup.x) return false;

		inf.y = (_min.y - o.y) / d.y;
		sup.y = (_max.y - o.y) / d.y;
		if (inf.y > sup.y) return false;

		inf.z = (_min.z - o.z) / d.z;
		sup.z = (_max.z - o.z) / d.z;
		if (inf.z > sup.z) return false;

		if (maxi(inf) > mini(sup)) {
			return false;
		}*/
		// TODO : tester le zFar

	}
} // namespace RT_ISICG
