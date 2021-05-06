#include <iostream>
#include <ngl/Image.h> 
#include <ngl/Random.h>>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Vec3.h>


#include "Arena.h"
#include "Player.h"
#include "RenderGlobals.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class Arena
/// \class to handle calls to gameplay objects,
///  for updating and drawing each tick. 


const static char* Player1 = "Player1";
const static char* Wall = "Wall";
const static char* Empty = "Empty";

Arena::Arena(unsigned int _w, unsigned int _h)
{
	// initialise arena with width and height, blank map
	m_width = _w;
	m_height = _h;
	m_items.resize(m_width * m_height);
	createDefaultObjects();
}

Arena::Arena(const std::string& _filename, size_t _numBoids)
{
	// initialise arena with map filled from an image file. 
	m_numBoids = _numBoids;
	// Load map image and create item map
	ngl::Image img(_filename);
	m_width = img.width();
	m_height = img.height();
	m_items.resize(m_width * m_height);
	createDefaultObjects();

	// Loop through each tile and set based on map
	for (unsigned int y = 0; y < m_height; ++y)
	{
		for (unsigned int x = 0; x < m_width; ++x)
		{
			if (img.getColour(x, y) == ngl::Vec4::zero())
			{
				setItem(x, y, ItemType::Wall, m_objects[Wall]);
			}
			else
			{
				setItem(x, y, ItemType::Empty, m_objects[Empty]);
			}
		}//loop collumns
	}//loop rows
	//set player
	setItem(4,4,ItemType::Player, m_objects[Player1]);


	// Generate Boids
	auto rng = ngl::Random::instance();
	rng->setSeed();
	m_boids.resize(m_numBoids);
	for (auto& b : m_boids)
	{
		ngl::Vec3 pos = (rng->getRandomPoint(m_width/2 - 8, m_height/2 - 8, 0));
		Vec2 pos2(pos.m_x + 4 + m_width / 2, pos.m_y + 4 + m_height / 2);
		Boid bb(pos2, m_width, m_height);
		b = bb;
		// direction
		ngl::Vec2 dir = rng->getRandomVec2();
		b.setDir(Vec2(dir.m_x, dir.m_y));
		std::cout << "BB:" << pos2.x << ' ' << pos2.y << '\n';
	}


	// Create floor plane
	ngl::VAOPrimitives::instance()->createTrianglePlane("floor", m_width, m_height, m_width, m_height,ngl::Vec3::up());
	ngl::VAOPrimitives::instance()->getVAOFromName("floor");
}


void Arena::createDefaultObjects()
{
	// Default objects to be referenced into the m_objects map grid.
	m_objects[Wall] = new GameObject();
	m_objects[Empty] = new GameObject();
	m_objects[Player1] = new Player(Vec2(8,8), m_width, m_height);
}

void Arena::keyEvent(int _x, int _y, int _xMouse, int _yMouse)
{
	// Convert Mouse coordinate to vector from centre of screen
	float xTotal = -(static_cast<float>(_xMouse) - RenderGlobals::getWindowWidth()/2)/400;
	float yTotal = -(static_cast<float>(_yMouse) - RenderGlobals::getWindowHeight()/2)/200;
	Vec2 movement = Vec2(xTotal, yTotal);

	//std::cout << movement.x << ' ' << movement.y <<'\n';
	m_objects[Player1]->moveEvent(movement);
}

void Arena::draw() const
{
	// Clear Command Line
	/*
	#ifdef WIN32
		system("cls");
	#else
		system("clear");
	#endif
	*/

	// Setup Shaders
	auto shader = ngl::ShaderLib::instance();
	shader->use(ngl::nglDiffuseShader);
	shader->setUniform("Colour", 1.0f, 0.0f, 0.0f, 1.0f);

	// Command Line Map Print & Wall Draw Calls
	size_t index = 0;
	for (size_t y = 0; y < m_height; ++y)
	{
		for (size_t x = 0; x < m_width; ++x)
		{
			switch (m_items[index++].type)
			{
			//case ItemType::Empty: std::cout << ' '; break;
			//case ItemType::Player: std::cout << 'P'; break;
			//case ItemType::Fruit: std::cout << 'F'; break;
			//case ItemType::Wall: std::cout << 'W'; drawWall(x, y); break;
			case ItemType::Wall: drawWall(x, y); break;
			
			}
		}
		//std::cout << '\n';
	}

	// Object Draw Calls
	for (auto& o : m_objects)
	{
		o.second->draw();
	}

	// Boid Draw Calls
	for (auto& b : m_boids)
	{
		b.draw();
	}
	// Draw floor
	shader->use(ngl::nglCheckerShader);
	ngl::Transformation tx;
	tx.setPosition(0.0f, -0.5f, 0.0f);
	shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
	ngl::VAOPrimitives::instance()->draw("floor");
}
void Arena::update(float _dt)
{
	std::cout << "update \n";

	// Object Update Calls
	for (auto& o : m_objects)
	{
		o.second->update(_dt);
	}

	// Player movement setup (creating new blank player class to work with)
	Player* p = static_cast<Player *>(m_objects[Player1]);
	auto pos = p->getPos();
	auto dir = p->getDir();
	Vec2 newPos = pos;

	// lamber function to get item at given tile coords for use in collision checks
	auto getItem = [=](unsigned int _x, unsigned int _y)
	{
		size_t index = _y * m_width + _x;
		return m_items[index].type;
	};


	// Player-Wall Collision Checks
	// x-axis
	if (dir.x != 0 && getItem(pos.x + dir.x, pos.y + dir.y) == ItemType::Empty)
	{
		newPos.x = pos.x + dir.x;
	}
	// y-axis
	if (dir.y != 0 && getItem(pos.x + dir.x, pos.y + dir.y) == ItemType::Empty)
	{
		newPos.y = pos.y + dir.y;
  }
	
  p->setPos(newPos);

	// Boid Update Calls
	// Player Collision
	
	m_boids.erase(
		std::remove_if(
			std::begin(m_boids),
			std::end(m_boids),
			[=](Boid b)
			{
				return playerBoidHit(b, *p, 1);
			}),
			std::end(m_boids)
				);

	// Boid-Boid collision
	for (auto& b : m_boids)
	{
		Vec2 avSep = Vec2(0.0f,0.0f);
		Vec2 avPos = Vec2(0.0f,0.0f);
		Vec2 avDir = Vec2(0.0f,0.0f);
		int noAvSep = 0;
		int noAvPos = 0;
		int noAvDir = 0;
		b.update(_dt);
		
		// Separation with Player
		if (playerBoidHit(b, * p, 5))
		{
			avSep = Vec2(p->getPos().x - b.getPos().x, p->getPos().y - b.getPos().y);
			noAvSep++;
			Vec2 bPos = b.getPos();
			Vec2 b2Pos = p->getPos();
			Vec2 di = b.getDir();
			b.setDir(Vec2(di.x - (b2Pos.x - bPos.x), di.y - (b2Pos.y - bPos.y)));
		}
		

		// Flock collisions with other boids
		for (auto& b2 : m_boids)
		{
			// Separation
			if (boidBoidHit(b, b2, 2))
			{
				avSep = Vec2(avSep.x + b2.getPos().x - b.getPos().x, avSep.y + b2.getPos().y - b.getPos().y);
				noAvSep++;
				Vec2 bPos = b.getPos();
				Vec2 b2Pos = b2.getPos();
				Vec2 di = b.getDir();
				b.setDir(Vec2(di.x - (b2Pos.x - bPos.x), di.y - (b2Pos.y - bPos.y)));
			}
			// Allignment
			if (boidBoidHit(b, b2, 4))
			{
				avDir = Vec2(avDir.x + b2.getDir().x, avDir.y + b2.getDir().y);
				noAvDir++;
			}
			// Cohesion
			if (boidBoidHit(b, b2, 3))
			{
				avPos = Vec2(avPos.x + b2.getPos().x, avPos.y + b2.getPos().y);
				noAvPos++;
			}
		}
		// Separation
		if (noAvSep > 0)
		{
			// divide vector by no. alligned
			avSep = Vec2((-avSep.x / noAvSep), (-avSep.y / noAvSep));
			// vector normalisation
			float vecLen = sqrt(avSep.x * avSep.x + avSep.y * avSep.y);
			avSep = Vec2(avSep.x / abs(vecLen), avSep.y / abs(vecLen));
			// add to main vector
			//b.setDir(Vec2(b.getDir().x + avSep.x, b.getDir().y + avSep.y));
		}

		// Allignment
		if (noAvDir > 0)
		{
			// divide vector by no. alligned
			avDir = Vec2(avDir.x / noAvDir, avDir.y / noAvDir);
			// vector normalisation
			float vecLen = sqrt(avDir.x * avDir.x + avDir.y * avDir.y);
			avDir = Vec2(avDir.x / abs(vecLen), avDir.y / abs(vecLen));
			// add to main vector
			b.setDir(Vec2(b.getDir().x + avDir.x, b.getDir().y + avDir.y));
		}
		

		// Cohesion
		if (noAvPos > 0)
		{
			// divide vector by no. cohesioning
			avPos = Vec2(avPos.x / noAvPos, avPos.y / noAvPos);
			// Get vector to centre of mass
			Vec2 boidToAvPos = Vec2(avPos.x - b.getPos().x, avPos.y - b.getPos().y);
			// vector normalisation
			float vecLen = sqrt(boidToAvPos.x * boidToAvPos.x + boidToAvPos.y * boidToAvPos.y);
			boidToAvPos = Vec2(boidToAvPos.x / abs(vecLen), boidToAvPos.y / abs(vecLen));
			// add to main vector
			//b.setDir(Vec2(b.getDir().x + boidToAvPos.x, b.getDir().y + boidToAvPos.y));
		}
		
	}

  //setItem(pos.x, pos.y, ItemType::Empty, m_objects[Empty]);
  //setItem(newPos.x, newPos.y, ItemType::Player, m_objects[Player1]);

}

bool Arena::playerBoidHit(Boid _b, Player _p, float _radius)
{
	// relative position
	Vec2 relPos(_p.getPos().x - _b.getPos().x, _p.getPos().y - _b.getPos().y);
	// distance
	float len = (relPos.x * relPos.x + relPos.y * relPos.y);

	if (len <= _radius * _radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Arena::boidBoidHit(Boid _b, Boid _b2, float _radius)
{
	// relative position
	Vec2 relPos(_b2.getPos().x - _b.getPos().x, _b2.getPos().y - _b.getPos().y);
	// distance
	float len = (relPos.x * relPos.x + relPos.y * relPos.y);

	if (len <= (_radius * _radius))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Arena::setItem(unsigned int _x, unsigned int _y, ItemType _type ,GameObject* _obj)
{
	// Set an item in the m_items map with given coords
  size_t index = _y * m_width + _x;
  m_items[index].type = _type;
  m_items[index].obj = _obj;
}

void Arena::drawWall(unsigned int _x, unsigned int _y) const
{
	// draw a wall block given x and y coordinated
	// screen midpoint
	float halfZ = -(m_height / 2.0f);
	float halfX = -(m_width / 2.0f);
	// transformation
	ngl::Transformation tx;
	tx.setPosition(halfX + _x, 0.0f, halfZ + _y);
	// shader
	auto shader = ngl::ShaderLib::instance();
	shader->setUniform("MVP", RenderGlobals::getVPMatrix() * tx.getMatrix());
	// draw
	ngl::VAOPrimitives::instance()->draw(ngl::cube);
}
