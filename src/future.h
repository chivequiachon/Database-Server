#ifndef __FUTURE_H
#define __FUTURE_H

//#include "servant.h"

#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>

template <typename T>
class Promise;

template <typename T>
class Future
{
private:
	long _refCounter;
	T _val;

	friend class Promise<T>;

public:
    Future() : _refCounter(0) {}
	Future(const T &val) : _refCounter(0), _val(val) {}
	~Future() {}

	T getValue() const { return _val; }
	void setValue(const T &val) { _val = val; }

	long getRefCount() const { return _refCounter; }
};

template <typename T>
class Promise
{
private:
	Future<T> *_f;
	std::mutex _dataReady;
	std::condition_variable _cv;

	bool _isReady;

public:
	Promise(const T mh) : _isReady(false)
	{
		// create future
		_f = new Future<T>(mh);
		_f->_refCounter = 1;
	}

	Promise() : _isReady(false)
	{
		// create future
		_f = new Future<T>(T());
		_f->_refCounter = 1;

	}

	Promise(const Promise &p) : _isReady(false)
	{
		_f = p._f;
		_isReady = p._isReady;
		_f->_refCounter++;
	}

	~Promise()
	{
		--_f->_refCounter;
		if(_f->_refCounter == 0)
			delete _f;
	}

	long getRefCount() const { return _f->getRefCount(); }

	Promise& operator=(const Promise &p)
	{
		// increment reference counter
		p._f->_refCounter++;
		
		// check if reference counter is greater than 1
		// if equal to one, delete underlying future object
		// the future object
		if(--_f->_refCounter <= 0)
			delete _f;

		_f = p._f;
		_isReady = p._isReady;
		return *this;
	}

	Promise& operator=(const T &mh)
	{
		// set value of future
		_f->setValue(mh);
		
		// notify condition variable that data is ready
		_isReady = true;
		_cv.notify_one();

		return *this;
	}

	Future<T>* operator->()
	{
		return _f;
	}

	T result(int timeOut, T valTimeOut, T valDefault)
	{
		T result;

		// create a unique lock and use it for the condition variable
		std::unique_lock<std::mutex> l(_dataReady);
		
		// wait until data is ready or time out
		bool flag = _cv.wait_for(l, std::chrono::milliseconds(timeOut), [this]()->bool{return _isReady;});

		// check if time out or data is ready
		result = !flag ? valTimeOut : _f->getValue();

		return result;
	}
};

#endif
