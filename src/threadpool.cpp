#include "threadpool.h"

#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
#include <iostream>

ThreadPool::ThreadPool()
{
	for(auto &a : t)
	{
		a = std::thread(
			[=]()->void
			{
				while(true)
				{
					task *task_ptr = NULL;
					{
						std::unique_lock<std::mutex>{lock};
						if(stop) break;
						if(task_queue.empty())
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));
							continue;
						}

						task_ptr = task_queue.front();
						task_queue.pop();
					}

					if(task_ptr)
					{
						do
						{
							if(task_ptr->isStopped) continue;
							task_ptr->task_func();
						}
						while(!(task_ptr->isKilled) && !stop);

						delete task_ptr;
					}
				}
			}
		);
	}
}

ThreadPool::~ThreadPool()
{
	{
	std::unique_lock<std::mutex>{lock};
	stop = true;
	}
	
	for(auto &a : t)
		a.join();
}

void ThreadPool::addTask(std::string id, std::function<void()> func, bool isRepititive)
{
	task *th_task = new task(id, func, isRepititive);
	task_queue.push(th_task);
	task_ptr_dict[id] = th_task;
}

void ThreadPool::addTask(std::string id, std::function<void()> func, bool isRepititive, bool isStopped)
{
	task *th_task = new task(id, func, isRepititive, isStopped);
	task_queue.push(th_task);
	task_ptr_dict[id] = th_task;
}

void ThreadPool::startTask(std::string id)
{
	task_ptr_dict[id]->start();
}

void ThreadPool::stopTask(std::string id)
{
	task_ptr_dict[id]->stop();
}

void ThreadPool::killTask(std::string id)
{
	task_ptr_dict[id]->kill();
	task_ptr_dict.erase(id);
}