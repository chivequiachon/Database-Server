#include "activation_list.h"
#include "method_request.h"

#include <list>
#include <mutex>
#include <thread>
#include <chrono>

void ActivationList::insert(MethodRequest *mr)
{
	(std::lock_guard<std::mutex>(_cs), _list.push_back(mr));
}

bool ActivationList::isEmpty()
{
    return _list.empty();
}

MethodRequest* ActivationList::pop()
{	
	std::lock_guard<std::mutex> l(_cs);
		MethodRequest *mr = _list.front();
    _list.pop_front();
	
    return mr;
}