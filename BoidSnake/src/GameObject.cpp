#include "GameObject.h"

/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Class GameObject
/// \template class for gameplay object to inherit from,
///  with draw and update and move functions. 

void GameObject::update(float _dt) 
{
 // blank, to be overriden by child classes
}
void GameObject::draw() const
{
  // blank, to be overriden by child classes 
}
void GameObject::moveEvent(const Vec2& _m)
{
  // blank, to be overriden by child classes
}