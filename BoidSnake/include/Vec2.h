#ifndef VEC23_H_
#define VEC23_H_
// Include Guard edited due to conflict with NGL Vec2 class

/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \struct Vec2
/// \a separate 2D vector struct. 


struct Vec2
{
	Vec2() = default;
	Vec2(float _x, float _y) : x{ _x }, y{ _y }{}
	float x;
	float y;
};

#endif // !VEC23_H_
