#ifndef _SYSTEMCOMPONENT_H
#define _SYSTEMCOMPONENT_H

#include <iterator>
#include <map>

template <typename K, typename V>
V GetWithDef(const std::map <K,V> & m, const K & key, const V defval)
{
   typename std::map<K,V>::const_iterator it = m.find( key );
   return it == m.end() ? defval : it->second;
}

/************************************************************
				SYSTEM COMPONENT INTERFACE
************************************************************/
class SystemComponent
{
public:
	inline explicit SystemComponent() {}
	virtual ~SystemComponent() { /* to be watched  */ }

	virtual bool run() = 0;
};

#endif