#ifndef CONQUEUE_H
#define CONQUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class ConcurrentQueue
{
public:
	inline explicit ConcurrentQueue()
	{
	}

	inline ~ConcurrentQueue()
	{
	}

	void enqueue(T el)
	{
		std::unique_lock<std::mutex> l(m_lock);
		container.push(el);
	}

	T dequeue(T defVal)
	{
		T returnVal = defVal;

		std::unique_lock<std::mutex> l(m_lock);
		{
			if(container.empty()) return returnVal;

			returnVal = container.front();
			container.pop();
		}

		return returnVal;
	}

private:
	std::mutex m_lock;
	std::queue<T> container;
};

#endif