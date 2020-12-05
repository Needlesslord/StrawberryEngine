#include "Globals.h"

class Texture
{
public:
	uint GetId() { return id; }
public:
	std::string texPath;
	uint id;
	uint w;
	uint h;

	std::string name;
	std::string fileName;

};