#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

class Component
{
public:

	enum Type
	{
		TYPE_NONE = -1,

		TYPE_MESH,
		TYPE_TEXTURE,
		TYPE_CAMERA
	};

public:

	Component(Type type, GameObject* go);
	virtual ~Component();

	virtual bool Update();

public:

	std::string name;
	Type type = Type::TYPE_NONE;
	GameObject* gameObject = nullptr;
	bool isActive = true;

};

#endif //__COMPONENT_H__