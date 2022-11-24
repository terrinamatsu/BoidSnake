#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_


#include "Vec2.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class GameObject
/// \Base class for all game objects,
///  including player and boid classes.


class GameObject
{
public:
	GameObject() = default;
	~GameObject() = default;
	virtual void update(float _dt);
	virtual void draw() const;
	virtual void moveEvent(const Vec2& _m);
private:

protected:

};

#endif // !GAMEOBJECT_H_
