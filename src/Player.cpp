#include <iostream>
#include <math.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>


#include "Player.h"
#include "RenderGlobals.h"

/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class Player
/// \handles the position, movment, and drawing of the player

Player::Player(const Vec2& _p, unsigned int _w, unsigned int _h) : GameObject()
{
	// Initialise player with position, map width and height, camera position,
	// and player model loaded from .obj file
	m_pos = _p;
	m_width = _w;
	m_height = _h;
	m_camPos = RenderGlobals::getCamPos();
	ngl::VAOPrimitives::instance()->loadObj("panda", "models/panda.obj");
}

void Player::update(float _dt) 
{
	// Unused 
}
void Player::draw() const 
{
	//std::cout << m_width << ' ' << m_pos.x << ' ' << m_height << ' ' << m_pos.y << '\n';

	// Draw Player
	// screen midpoint
	float halfZ = -(m_height / 2.0f);
	float halfX = -(m_width / 2.0f);
	// transformations
	ngl::Transformation tx;
	tx.setPosition(halfX + m_pos.x - 0.5, 0.0f, halfZ + m_pos.y - 0.5);
	// shaders
	auto shader = ngl::ShaderLib::instance();
	shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
	shader->setUniform("Colour", 0.0f, 1.0f, 0.0f, 1.0f);


	/*
	//* Old code, was used to try and get the on screen value of the player position as they moved about the map. 
	//* Scrapped in favour of moving camera following the player, centred on screen. 

	ngl::Vec3 objToCam = ngl::Vec3(halfX + m_pos.x, 0, halfZ + m_pos.y) - m_camPos;
	ngl::Vec3 centreToCam = ngl::Vec3() - m_camPos; // looking at pos - cam pos

	float top = (objToCam.m_x * centreToCam.m_x) + (objToCam.m_y * centreToCam.m_y) + (objToCam.m_z * centreToCam.m_z);
	float objLen = pow(objToCam.m_x, 2) + pow(objToCam.m_y, 2) + pow(objToCam.m_z, 2);
	float centreLen = pow(centreToCam.m_x, 2) + pow(centreToCam.m_y, 2) + pow(centreToCam.m_z, 2);

	objLen = abs(sqrt(objLen));
	centreLen = abs(sqrt(centreLen));

	float value = acosf(top / (objLen * centreLen));
	float valueDeg = (value / (3.14159f)) * 180.0f;

	int pix = static_cast<int>((valueDeg / 22.5) * 1024);

	std::cout << pix << ' ' << m_pos.x << ' ' << m_pos.y << '\n';
	*/

	RenderGlobals::setView(ngl::Vec3(halfX + m_pos.x, m_camPos.m_y, m_camPos.m_z + halfZ + m_pos.y), ngl::Vec3(halfX + m_pos.x, 0.0f, halfZ + m_pos.y), { 0.0f,0.0f,1.0f });

	ngl::VAOPrimitives::instance()->draw("panda");
}
void Player::moveEvent(const Vec2& _m) 
{
	//std::cout << _m.x << ' ' << _m.y << '\n';
	
	ngl::Vec2 toNormal(_m.x, _m.y);
	// Clamp speed to max 1 and min -1 in x and y
	if (_m.x > 1 || _m.x < -1 || _m.y > 1 || _m.y < -1)
	{
		toNormal.normalize();
	}
	
	//std::cout << toNormal << '\n';
	m_dir = Vec2(toNormal.m_x, toNormal.m_y);

}