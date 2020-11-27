class Texture
{
public:
	uint GetId() { return id; }
public:
	const char* texPath = nullptr;
	char* name = nullptr;
	uint id;
	uint w;
	uint h;

	bool isActive = true;
};