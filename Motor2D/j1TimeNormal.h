#ifndef __j1TIMENORMAL_H__
#define __j1TIMENORMAL_H__

#include "p2Defs.h"

class j1TimeNormal
{
public:

	// Constructor
	j1TimeNormal();

	void Start();
	uint32 Read() const;
	float ReadSec();

private:
	uint32	started_at;
};

#endif //__j1TIMENORMAL_H__
