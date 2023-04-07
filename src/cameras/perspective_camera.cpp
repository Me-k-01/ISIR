#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt, // point que regarde la caméra
										  const Vec3f & p_up, //p_up initialisé au vecteur up de l'espace monde
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		/// TODO ! _u ? _v ? _w ?
		_w = glm::normalize(p_position - p_lookAt); // Inverse de la direction d'observation = - (p_lookAt - p_position)
		_u = glm::normalize(glm::cross(p_up, _w)); 
		_v = glm::normalize(glm::cross(_w, _u));



		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		/// TODO ! _viewportTopLeftCorner ?	_viewportU ? _viewportV ?

		float o = (glm::tan(glm::radians(_fovy) / 2.f) * _focalDistance); // TOA
		_viewportV = _v * o * 2.f; // Hauteur
		_viewportU = _u * _aspectRatio * o * 2.f; // Largeur
		Vec3f milieu = _position + _focalDistance * -_w;
		_viewportTopLeftCorner = milieu + _viewportV / 2.f + -_viewportU / 2.f;
		 
	}

} // namespace RT_ISICG
