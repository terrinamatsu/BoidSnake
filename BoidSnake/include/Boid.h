#ifndef BOID_H_
#define BOID_H_


#include <ngl/Vec3.h>


#include "GameObject.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \class Boid
/// \Child of GameObject, an individual boid
///  from the flock. 
///  Handles position, movement, and drawing.


class Boid : public GameObject
{
public:
  Boid() = default;
  ~Boid() = default;
  Boid(const Vec2& _p, unsigned int _w, unsigned int _h, float _grab = 1);
  virtual void  update(float _dt) override;
  virtual void  draw() const override;
  Vec2          getPos() const { return m_pos; }
  Vec2          getDir() const { return m_dir; }
  void          setPos(const Vec2& _p) { m_pos = _p; }
  void          setDir(const Vec2& _d) { m_dir = _d; }
private:
  std::vector<Boid> m_boids;
  float         m_influenceSphereRadius;
  float         m_grabSphereRadius;
  unsigned int  m_width;
  unsigned int  m_height;
  Vec2          m_pos;
  Vec2          m_dir;
};

#endif // !BOID_H_

