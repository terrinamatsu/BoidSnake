#ifndef RENDERGLOBALS_H_
#define RENDERGLOBALS_H_


#include <ngl/Mat4.h>
#include <ngl/Vec3.h>


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \class RenderGlobals
/// \holds all the variables to do with rendering, such as the view and projection matrices
///  for easy access anywhere in code.


class RenderGlobals
{
public:
	RenderGlobals() = delete;
	~RenderGlobals() = delete;
	static void				setView(const ngl::Vec3& _eye, const ngl::Vec3 &_look, const ngl:: Vec3 & _up);
	static void				setProjection(float _fov, float _aspect, float _near = 0.1f, float _far = 100.0f);
	static void				setWindow(int _width, int _height);

	static ngl::Mat4	getViewMatrix();
	static ngl::Mat4	getProjectMatrix();
	static ngl::Mat4	getVPMatrix();
	static ngl::Vec3	getCamPos();
	static int				getWindowWidth();
	static int				getWindowHeight();

private:
	static ngl::Mat4	s_view;
	static ngl::Mat4	s_project;
	static ngl::Vec3	s_camPos;
	static int				s_windowWidth;
	static int				s_windowHeight;
};


#endif // !RENDERGLOBALS_H_
