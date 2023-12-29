#ifndef COMPONENT_H
#define COMPONENT_H
#include "game_object.h"
class GameObject;
class Component
{
public:
	Component();
	virtual ~Component();
	GameObject* game_object() { return game_object_; };
	void set_game_object(GameObject* game_object) { game_object_ = game_object; };
private:
	GameObject* game_object_;
};
#endif // !COMPONENT_H
