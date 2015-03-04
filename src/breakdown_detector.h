#ifndef BRDETECT_H
#define BRDETECT_H

#include "Server.h"
#include "systemcomponent.h"

class BreakdownDetector : public SystemComponent
{
	IServer *s;

public:
	inline explicit BreakdownDetector(IServer *s) : s(s) {}
	virtual ~BreakdownDetector() {}

	virtual bool run();
	virtual bool run(int *time);
};

#endif