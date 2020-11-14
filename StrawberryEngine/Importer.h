#ifndef __Importer_H__
#define __Importer_H__

#include "Globals.h"

class Importer
{
public:
	Importer();
	virtual ~Importer();

	virtual bool Init();
	virtual bool Start();
	virtual update_status PreUpdate(float dt);
	virtual update_status PostUpdate(float dt);
	virtual bool CleanUp();

	virtual void Import();
	virtual uint64 Save();
	virtual bool Load();

public:

};

#endif // __Importer_H__