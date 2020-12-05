#include "Component.h"

Component::Component(Type type, GameObject* go)
{
	this->type = type;
	this->gameObject = go;
}

Component::~Component()
{

}

bool Component::Update()
{
	return true;
}
