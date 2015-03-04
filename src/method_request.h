#ifndef __METHOD_REQUEST_H
#define __METHOD_REQUEST_H

#include "servant.h"
#include "future.h"

class MethodRequest
{
public:
	virtual ~MethodRequest() {}
	virtual void call() = 0;
	virtual bool isRunnable() = 0;
};

// FOR SERVANT
template <typename T_Ds, typename T_Input>
class MREnqueue : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input _mh;

public:
	MREnqueue(T_Ds *mq, const T_Input &mh) : _mq(mq), _mh(mh) {}
	virtual ~MREnqueue() {}

	bool isRunnable() { return true; }
	void call() { _mq->enqueue(_mh); }
};

template <typename T_Ds, typename T_Input>
class MRDequeue : public MethodRequest
{
private:
	T_Ds *_mq;
	Promise<T_Input> &_p;

public:
	MRDequeue(T_Ds *mq, Promise<T_Input> &p) : _mq(mq), _p(p) {}
	virtual ~MRDequeue() {}

	bool isRunnable() { return !_mq->empty(); }
	void call() { _p = _mq->dequeue(); }
};

// FOR MAP SERVANT
template <typename T_Ds, typename T_Input1, typename T_Input2>
class MRSetElement : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input1 _mh;
	T_Input2 _mh2;

public:
	MRSetElement(T_Ds *mq, const T_Input1 &mh, const T_Input2 &mh2) :
		_mq(mq), _mh(mh), _mh2(mh2) {}
	virtual ~MRSetElement() {}

	bool isRunnable() { return true; }
	void call() { _mq->setElement(_mh, _mh2); }
};

template <typename T_Ds, typename T_Input1, typename T_Input2>
class MRGetValue : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input1 _mh;
	T_Input2 _defmh;
	Promise<T_Input2> &_p;

public:
	MRGetValue(T_Ds *mq, const T_Input1 &mh, const T_Input2 &defmh, Promise<T_Input2> &p) :
		_mq(mq), _mh(mh), _defmh(defmh), _p(p) {}

	virtual ~MRGetValue() {}

	bool isRunnable() { return true; }
	void call() { _p = _mq->getValue(_mh, _defmh); }
};

template <typename T_Ds, typename T_Input1, typename T_Input2>
class MRGetKey : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input2 _mh;
	T_Input1 _defmh;
	Promise<T_Input1> &_p;

public:
	MRGetKey(T_Ds *mq, const T_Input2 &mh, const T_Input1 &defmh, Promise<T_Input1> &p) :
		_mq(mq), _mh(mh), _defmh(defmh), _p(p) {}

	virtual ~MRGetKey() {}

	bool isRunnable() { return true; }
	void call() { _p = _mq->getKey(_mh, _defmh); }
};

template <typename T_Ds, typename T_Input1, typename T_Input2>
class MRGetValueEnhanced : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input1 _mh;
	T_Input2 _defmh;
	Promise<T_Input2> &_p;

public:
	MRGetValueEnhanced(T_Ds *mq, const T_Input1 &mh, const T_Input2 &defmh, Promise<T_Input2> &p) :
		_mq(mq), _mh(mh), _defmh(defmh), _p(p) {}

	virtual ~MRGetValueEnhanced() {}

	bool isRunnable() { return true; }
	void call() { _p = _mq->template getValueEnhanced<T_Input1>(_mh, _defmh); }
};

// FOR LIST SERVANT
template <typename T_Ds, typename T_Input>
class MRPushBack : public MethodRequest
{
private:
	T_Ds *_mq;
	T_Input _mh;

public:
	MRPushBack(T_Ds *mq, const T_Input &mh) : _mq(mq), _mh(mh) {}
	virtual ~MRPushBack() {}

	bool isRunnable() { return true; }
	void call() { _mq->push_back(_mh); }
};

template <typename T_Ds, typename T_Input>
class MRPopBack : public MethodRequest
{
private:
	T_Ds *_mq;
	Promise<T_Input> &_p;

public:
	MRPopBack(T_Ds *mq, Promise<T_Input> &p) : _mq(mq), _p(p) {}
	virtual ~MRPopBack() {}

	bool isRunnable() { return !_mq->empty(); }
	void call() { _p = _mq->pop_back(); }
};

#endif
