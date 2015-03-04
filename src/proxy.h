#ifndef __PROXY_H
#define __PROXY_H

#include "servant.h"
#include "scheduler.h"
#include "future.h"
#include "method_request.h"
#include "threadpool.h"

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <list>
#include <map>
#include <iostream>
#include <string>

template <typename T_Queue, typename T_Handler>
class ServantProxy
{
private:
	T_Queue queue;
	Scheduler s;

public:
	inline explicit ServantProxy(ThreadPool *t, std::string id) : s(t) { s.run(id); }
	
	inline explicit ServantProxy() {}
	
    void setThreadPool(ThreadPool *t, std::string id)
    {
		s.setThreadPool(t);
		s.run(id);
    }

	void enqueue(const T_Handler &handler)
	{
		MethodRequest *mr_enqueue = new MREnqueue<T_Queue, T_Handler>(&queue, handler);
		s.insert(mr_enqueue);
	}

	Promise<T_Handler> dequeue()
	{
		Promise<T_Handler> result;
		MethodRequest *mr_dequeue = new MRDequeue<T_Queue, T_Handler>(&queue, result);
		s.insert(mr_dequeue);
		return result;
	}

	bool empty() const
	{
		return queue.empty();
	}
};

template <typename T_List, typename T_Handler>
class ListServantProxy
{
private:
	T_List l;
	Scheduler s;

public:
	inline explicit ListServantProxy(ThreadPool *t, std::string id) : s(t) { s.run(id); }
	
	inline explicit ListServantProxy() {}
	
	std::list<T_Handler>* getList()
	{
		return l.getList();
	}

    void setThreadPool(ThreadPool *t, std::string id)
    {
		s.setThreadPool(t);	
		s.run(id);
    }

	void push_back(const T_Handler &handler)
	{
		MethodRequest *mr_push_back = new MRPushBack<T_List, T_Handler>(&l, handler);
		s.insert(mr_push_back);
	}

	Promise<T_Handler> pop_back()
	{
		Promise<T_Handler> result;
		MethodRequest *mr_pop_back = new MRPopBack<T_List, T_Handler>(&l, result);
		s.insert(mr_pop_back);
		return result;
	}

	bool empty() const
	{
		return l.empty();
	}
};

template <typename T_Map, typename T_Key, typename T_Value>
class MapServantProxy
{
private:
	T_Map map;
	Scheduler s;

public:
	inline explicit MapServantProxy(ThreadPool *t, std::string id) : s(t) { s.run(id); }
	
	inline explicit MapServantProxy() {}
	
	std::map<T_Key, T_Value>* getMap()
	{
		return map.getMap();
	}

    void setThreadPool(ThreadPool *t, std::string id)
    {
		s.setThreadPool(t);	
		s.run(id);
    }

    bool empty() const
	{
		return map.empty();
	}

	void setElement(const T_Key &key, const T_Value &value)
	{
		MethodRequest *mr_set = new MRSetElement<T_Map, T_Key, T_Value>(&map, key, value);
		s.insert(mr_set);
	}

	Promise<T_Value> getValue(const T_Key &key, const T_Value &defValue)
	{
		Promise<T_Value> result;
		MethodRequest *mr_getValue =
			new MRGetValue<T_Map, T_Key, T_Value>(&map, key, defValue, result);
		s.insert(mr_getValue);
		return result;
	}

	template<typename Type>
	Promise<T_Value> getValueEnhanced(const Type &key, const T_Value &defValue)
	{
		Promise<T_Value> result;
		MethodRequest *mr_getValue =
			new MRGetValueEnhanced<T_Map, Type, T_Value>(&map, key, defValue, result);
		s.insert(mr_getValue);
		return result;
	}

	Promise<T_Key> getKey(const T_Value &val, const T_Key &defKey)
	{
		Promise<T_Key> result;
		MethodRequest *mr_getKey =
			new MRGetKey<T_Map, T_Value, T_Key>(&map, val, defKey, result);
		s.insert(mr_getKey);
		return result;
	}
};

#endif
