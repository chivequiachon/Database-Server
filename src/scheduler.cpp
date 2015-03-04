#include "scheduler.h"
#include "threadpool.h"

#include <thread>
#include <string>

void Scheduler::setThreadPool(ThreadPool *t)
{
	this->t = t;
}

void Scheduler::insert(MethodRequest *mr)
{
	_al.insert(mr);
}

void Scheduler::run(std::string id)
{
	t->addTask(id,
		[&]()->void
		{
		    //while(true)
			//{
				if(!_al.isEmpty())
				{
					MethodRequest *current = _al.pop();	

					if(current->isRunnable())
					{
						current->call();
						delete current;
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			//}
		}, 1
	);
}