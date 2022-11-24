#ifndef ARENA_H_
#define ARENA_H_


#include <string>
#include <unordered_map>
#include <vector>


#include "Boid.h"
#include "GameItem.h"
#include "Player.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \class Arena
/// \Handles all game objects, and calls to each every tick.


class Arena
{
public:
	Arena() = default;
	~Arena() = default;
	Arena(unsigned int _w, unsigned int _h);
	Arena(const std::string& _filename, size_t _numBoids = 100);
	void																					draw() const;
	void																					update(float _dt);
	void																					keyEvent(int _x, int _y, int _xMouse, int _yMouse);

private:
	void																					setItem(unsigned int _x, unsigned int _y, ItemType _type, GameObject * _obj = nullptr);
	void																					createDefaultObjects();
	void																					drawWall(unsigned int _x, unsigned int _y) const;
	bool																					playerBoidHit(Boid _b, Player _p, float _radius);
	bool																					boidBoidHit(Boid _b, Boid _b2, float _radius);
	unsigned int																	m_width=0;
	unsigned int																	m_height=0;
	std::vector<GameItem>													m_items;
	std::vector<Boid>															m_boids;
	std::unordered_map<std::string, GameObject *> m_objects;
	size_t																				m_numBoids = 10;
};

#endif // !ARENA_H_
