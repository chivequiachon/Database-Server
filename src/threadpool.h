#ifndef __THREAD_POOL
#define __THREAD_POOL

#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
#include <unordered_map>
#include <string>

class ThreadPool
{
    class task
    {
        std::mutex lock;
        std::string id;
        std::function<void()> task_func;
        
        bool isStopped = false;
        bool isRepetitive = false;
        bool isKilled = false;

    public:
        task(std::string id, std::function<void()> task_func, bool repetitive) :
            id(id), task_func(task_func), isRepetitive(repetitive)
        {
        }

        task(std::string id, std::function<void()> task_func, bool repetitive, bool stopped) :
            task(id, task_func, repetitive)
        {
             isStopped = stopped;
        }

        void stop()
        {
            std::unique_lock<std::mutex>{lock};
            isStopped = true;
        }

        void start()
        {
            std::unique_lock<std::mutex>{lock};
            isStopped = false;  
        }

        void kill()
        {
            std::unique_lock<std::mutex>{lock};
            isKilled = true;
        }

    protected:
        friend class ThreadPool;
    };

    std::thread t[15];
    std::mutex lock;
    bool stop = false;

    std::unordered_map<std::string, task*> task_ptr_dict;
    std::queue<task*> task_queue;

public:
    explicit ThreadPool();
    ~ThreadPool();

    void addTask(std::string id, std::function<void()> func, bool isRepititive);
    void addTask(std::string id, std::function<void()> func, bool isRepititive, bool isStopped);

    void startTask(std::string id);
    void stopTask(std::string id);
    void killTask(std::string id);
};

#endif
