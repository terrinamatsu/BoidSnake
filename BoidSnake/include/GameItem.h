#ifndef GAMEITEM_H_
#define GAMEITEM_H_


#include "GameObject.h"


/// \author Jacob Worgan
/// \version 1.0
/// \15/6/2020

/// \Struct GameItem
/// \Holds the value of one of four enum types, 
///  plus a reference to a GameObject.


enum class ItemType{Player,Fruit,Wall,Empty};


struct GameItem
{
	ItemType		type = ItemType::Empty;
	GameObject* obj = nullptr;
};

#endif // ! GAMEITEM_H_
