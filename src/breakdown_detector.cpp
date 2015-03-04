#include "breakdown_detector.h"

bool BreakdownDetector::run() { return false; }
bool BreakdownDetector::run(int *time)
{
	if((*time) - 1 < 0)
		return false;

	(*time)--;			
	return true;
}