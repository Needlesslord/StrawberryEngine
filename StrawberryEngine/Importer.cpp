#include "Importer.h"

Importer::Importer()
{

}

Importer::~Importer()
{

}

bool Importer::Init()
{
	return true;
}

bool Importer::Start()
{
	return true;
}

update_status Importer::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status Importer::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool Importer::CleanUp()
{
	return true;
}

void Importer::Import()
{

}

uint64 Importer::Save()
{
	uint64 ret;

	return ret;
}

bool Importer::Load()
{
	return true;
}