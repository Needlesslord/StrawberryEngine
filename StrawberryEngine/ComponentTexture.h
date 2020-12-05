#ifndef __COMPONENT_TEXTURE_H__
#define __COMPONENT_TEXTURE_H__

#include "Component.h"
#include "Texture.h"

class ComponentTexture : public Component
{
public:

	ComponentTexture(Type type, GameObject* go);
	~ComponentTexture();

public:

	Texture* texture = nullptr;

};

#endif //__COMPONENT_TEXTURE_H__