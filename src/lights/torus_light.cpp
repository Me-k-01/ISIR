#include "torus_light.hpp" 
#include "utils/random.hpp" 

namespace RT_ISICG
{
	LightSample TorusLight::sample(const Vec3f& p_point) const {
		Vec3f posSurf;
		Vec3f normal;
		_rdmPoint(posSurf, normal); 
		const Vec3f neg_wi = posSurf - p_point; // = - wi
		
		//if (glm::length(normal)  == 0.f) std::cout << "ERR\n";
		//if (glm::length(neg_wi)  == 0.f) std::cout << "ERR\n";
		//if (glm::length(posSurf) == 0.f) std::cout << "ERR\n";

		const Vec3f dir = glm::normalize(neg_wi);
		const float dist = glm::length(neg_wi);

		float cosThetaI = glm::dot(dir, normal);
		// On d�termine le cos Th�ta suivant le sens du rayon par rapport � la normal 		
		if (cosThetaI < 0.f) {
			// Car on peut tapper la surface depuis l'interieur de la lumi�re (pour �clairer l'objet qui �met de la lumi�re par example)
			cosThetaI = -cosThetaI;
		}
		const float pdf = dist * dist / (cosThetaI * _area);

		const Vec3f rad = _color * _power / pdf;
		return LightSample(dir, dist, rad, pdf);
	}
	
	void TorusLight::_rdmPoint(Vec3f & p_point, Vec3f & p_normal) const { // Calcul un point al�atoire sur le torus distribu� de mani�re homog�ne
		// source : https://math.stackexchange.com/questions/2017079/uniform-random-points-on-a-torus
		

		float theta = TWO_PIf * randomFloat();
		float phi = TWO_PIf * randomFloat();
		float rTCosTheta = _radius + _thickness * cos(theta);

		// Rejection sampling 
		while (randomFloat() > (rTCosTheta / (_radius + _thickness))) {
			theta = TWO_PIf * randomFloat(); 
			phi = TWO_PIf * randomFloat();
			rTCosTheta = _radius + _thickness * cos(theta);
		}
		
		p_point = Vec3f( 
			rTCosTheta * cos(phi),
			rTCosTheta * sin(phi),
			_thickness * sin(theta)
		) + _center;

		// On ajoute 1 � la thickness pour trouver la direction de la normal 
		// a + (x+1)b
		// a + xb + b
		// Ce qui reviens � ajouter le cosTheta pour trouver le second point qui definit la direction de la normal
		//const float rTCosThetaNorm = rTCosTheta + cos(theta);
		const float rTCosThetaNorm = _radius + (_thickness+1.f) * cos(theta);
		Vec3f out = Vec3f(
			rTCosThetaNorm * cos(phi), 
			rTCosThetaNorm * sin(phi),
			(_thickness + 1.f) * sin(theta)
		) + _center;
				
		p_normal = normalize(out - p_point);
		//if (length(p_normal) == 0.f) 
		//	std::cout << "ERR\n";
		
	}

	/*
	void TorusLight::_rdmPoint(Vec3f & p_point, Vec3f & p_normal) const { // Calcul un point al�atoire sur le torus distribu� de mani�re homog�ne
		float u; float v; float w;
		while (1) {
			u = randomFloat();
			v = randomFloat();
			w = randomFloat();

			const float theta = TWO_PIf * u;
			const float phi = TWO_PIf * v;

			if (w <= ((_radius + _thickness * cos(theta)) / (_radius + _thickness))) {
				p_point = Vec3f(
					(_radius + _thickness * cos(theta)) * cos(phi),
					(_radius + _thickness * cos(theta)) * sin(phi),
					_thickness * sin(theta)
				) + _center;
				Vec3f out = Vec3f(
					(_radius + (_thickness + 1.f) * cos(theta)) * cos(phi),
					(_radius + (_thickness + 1.f) * cos(theta)) * sin(phi),
					(_thickness + 1.f) * sin(theta)
				) + _center;
				p_normal = normalize(out - p_point);
				return;
			}
		}
	}
	*/
}