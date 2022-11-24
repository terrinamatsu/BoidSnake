#ifndef PLAYER_H_
#define PLAYER_H_


#include <ngl/Vec3.h>


#include "GameObject.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \class Player
/// \child of GameObject, handles the player character,
///  including position, movement, and drawing.


class Player : public GameObject
{
public:
	Player(const Vec2& _p, unsigned int _w, unsigned int _h);
	virtual void	update(float _dt) override;
	virtual void	draw() const override;
	virtual void	moveEvent(const Vec2& _m) override;
	Vec2					getPos() const { return m_pos; }
	Vec2					getDir() const { return m_dir; }
	void					setPos(const Vec2& _p) { m_pos = _p; }
private:

	ngl::Vec3			m_camPos;
	unsigned int	m_width;
	unsigned int	m_height;
	Vec2					m_pos;
	Vec2					m_dir;
};


#endif // !PLAYER_H_
