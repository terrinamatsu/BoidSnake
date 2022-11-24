#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>


#include "Boid.h"
#include "RenderGlobals.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class Boid
/// \class to handle individual boids,
///  for movement, position, and drawing. 


Boid::Boid(const Vec2& _p, unsigned int _w, unsigned int _h, float _grab) : GameObject()
{
	// Create boid
	// Initialise position, map width and height, and the boid's radius.
	m_pos = _p;
	m_width = _w;
	m_height = _h;
	m_grabSphereRadius = _grab;
	//ngl::VAOPrimitives::instance()->loadObj("Dory", "models/dory.obj");
}

void Boid::update(float _dt) 
{
	// Check for bounds
	if (m_pos.x < 0 || m_pos.x > m_width || m_pos.y < 0 || m_pos.y > m_height)
	{
		m_dir = Vec2(m_width/2 - m_pos.x, m_height/2 - m_pos.y);
	}

	// Normalise Direction Vector
	float vecLen = sqrt(m_dir.x * m_dir.x + m_dir.y * m_dir.y);
  m_dir = Vec2(m_dir.x / abs(vecLen), m_dir.y / abs(vecLen));

	//std::cout << m_pos.x << '\n';

	// move pos in direction of dir, scaled with _dt
	m_pos.x += m_dir.x * _dt;
	m_pos.y += m_dir.y * _dt;
}
void Boid::draw() const 
{
	//Draw boid to screen.
	
	// screen midpoint
	float halfZ = -(m_height / 2.0f);
	float halfX = -(m_width / 2.0f);
	// transformations
	ngl::Transformation tx;
	tx.setPosition(halfX + m_pos.x, 0.0f, halfZ + m_pos.y);
	// shaders
	auto shader = ngl::ShaderLib::instance();
	shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
	shader->setUniform("Colour", 0.0f, 0.0f, 1.0f, 1.0f);

	// draw
	ngl::VAOPrimitives::instance()->draw(ngl::cube);
}