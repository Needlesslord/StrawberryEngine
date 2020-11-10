#ifndef __Importer_H__
#define __Importer_H__

#include "Globals.h"

class Importer
{
public:
	Importer();
	~Importer();

	bool Import();
	bool Save();
	bool Load();

};

#endif // __Importer_H__