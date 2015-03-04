#ifndef __ACTIVATION_LIST_H
#define __ACTIVATION_LIST_H

#include "method_request.h"

#include <list>
#include <mutex>

class ActivationList
{
private:
	std::list<MethodRequest*> _list;
	std::mutex _cs; // used to determine critical sections

	friend class Scheduler;
public:
	inline explicit ActivationList() : _cs()
	{
	}

	inline ~ActivationList()
	{
	}

	void insert(MethodRequest *mr);
    bool isEmpty();

	MethodRequest* pop();
};

#endif
