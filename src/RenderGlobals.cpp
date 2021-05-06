#include "RenderGlobals.h"
#include <ngl/Util.h>

ngl::Mat4 RenderGlobals::s_view = ngl::Mat4();
ngl::Mat4 RenderGlobals::s_project = ngl::Mat4();
ngl::Vec3 RenderGlobals::s_camPos = ngl::Vec3();
int RenderGlobals::s_windowWidth = 0;
int RenderGlobals::s_windowHeight = 0;


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class RenderGlobals
/// \class to store and handle view and projection matrices


void RenderGlobals::setView(const ngl::Vec3& _eye, const ngl::Vec3& _look, const ngl::Vec3& _up)
{
	s_camPos = _eye;
	s_view = ngl::lookAt(_eye, _look, _up);
}

void RenderGlobals::setProjection(float _fov, float _aspect, float _near, float _far)
{
	s_project = ngl::perspective(_fov, _aspect, _near, _far);
}

void RenderGlobals::setWindow(int _width, int _height)
{
	s_windowWidth = _width;
	s_windowHeight = _height;
}

ngl::Mat4 RenderGlobals::getViewMatrix()
{
	return s_view;
}
ngl::Mat4 RenderGlobals::getProjectMatrix()
{
	return s_project;
}
ngl::Mat4 RenderGlobals::getVPMatrix()
{
	return s_project * s_view;
}
ngl::Vec3 RenderGlobals::getCamPos()
{
	return s_camPos;
}

int RenderGlobals::getWindowWidth()
{
	return s_windowWidth;
}

int RenderGlobals::getWindowHeight()
{
	return s_windowHeight;
}