#ifndef __SERVANT_H
#define __SERVANT_H

#include <iostream>
#include <queue>
#include <map>
#include <list>
#include <string>
#include <cstring>

template <typename T_Key, typename T_Val>
class MapServant
{
private:
	std::map<T_Key, T_Val> mymap;

private:
	template <typename K, typename V>
	V GetWithDef(const std::map <K,V> & m, const K & key, const V & defval)
	{
	   typename std::map<K,V>::const_iterator it = m.find( key );
	   return it == m.end() ? defval : it->second;
	}

public:
	inline explicit MapServant() {}
	inline ~MapServant() {}

	std::map<T_Key, T_Val>* getMap()
	{
		return &mymap;
	}

	void setElement(T_Key key, T_Val value)
	{
		mymap[key] = value;
	}

	T_Val getValue(T_Key key, T_Val defValue) const
	{
		return GetWithDef(mymap, key, defValue);
	}

	T_Key getKey(T_Val value, T_Key defValue) const
	{
		// still buggy
		for(auto& a : mymap)
		{
			if(a.second == value)
				return a.first;
		}

		return defValue;
	}

	template<typename Type>
	T_Val getValueEnhanced(Type key, T_Val defValue) const
	{
		for(auto a : mymap)
		{
			if(a.second == key)
				return a.second;
		}

		return defValue;
	}

	bool empty() const { return mymap.empty(); }
};

template <typename T_Handler>
class ListServant
{
private:
	std::list<T_Handler> container;

public:
	inline explicit ListServant() {}
	inline ~ListServant() {}

	bool empty() const { return container.empty(); }
	void push_back(const T_Handler handler)
	{
		container.push_back(handler);
	}

	T_Handler pop_back()
	{
		T_Handler result;
		if(!container.empty)
		{
			result = container.back();
			container.pop_back();
		}
	}

	std::list<T_Handler>* getList()
	{
		return &container;
	}
};

template <typename T_Handler>
class Servant
{
private:
	std::queue<T_Handler> queue;

public:
	inline explicit Servant() {}
	inline ~Servant() {}

	bool empty() const { return queue.empty(); }
	void enqueue(const T_Handler handler)
	{
		queue.push(handler);
	}

	T_Handler dequeue()
	{
		T_Handler result;
		if(!queue.empty())
		{
			result = queue.front();
			queue.pop();
		}

		return result;
	}
};

#endif